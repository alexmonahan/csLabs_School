table = read.csv('NBA_record.csv')
teams = read.csv('teams.txt', header=FALSE, as.is=TRUE) num_games = nrow(table)
num_teams = nrow(teams)

loglik = function(theta, Home, Away, Y) {
  alpha = theta[1]
  beta = c(0, theta[-1])
  params = alpha + beta[Home] - beta[Away] return(sum(Y * params - log(1 + exp(params))))
}

theta0 = rep(0, num_teams) result = optim(theta0, loglik,
                                          Home=table$Home, Away=table$Away, Y=table$Y, method='BFGS', control=list('fnscale'=-1))


coefs = c(0, result$par[-1])
ranking = order(coefs, decreasing=TRUE) data.frame(team=teams[ranking[1:8],],
                                                   score=coefs[ranking[1:8]])
##Here are the eight teams with the highest Bradley Terry scores
##                       team       score
## 1 10 Golden State Warriors  1.90274553
##...
## 5 21 Oklahoma City Thunder  0.42045913
## 6  13 Los Angeles Clippers  0.28898908
## 7          1 Atlanta Hawks  0.00000000
## 8            16 Miami Heat -0.01215086

result$par[1] #0.46 --- Home court advantage factor

#Log-likelihood
loglik_noalpha = function(theta, Home, Away, Y) { beta = c(0, theta)
params = beta[Home] - beta[Away]
return(sum(Y * params - log(1 + exp(params))))
}
theta0 = rep(0, num_teams - 1)
result_noalpha = optim(theta0, loglik_noalpha,
                       Home=table$Home, Away=table$Away, Y=table$Y, method='BFGS', control=list('fnscale'=-1))

#optimal log likelihood values
print(result$value) ## [1] -680.2417 
print(result_noalpha$value) ## [1] -705.08


#Generalized likelihood ratio testInheritedMethods(
statistic = -2 * (result_noalpha$value - result$value) p_value = 1 - pchisq(statistic, df=1)
print(statistic)
  ## [1] 49.67658 
print(p_value)
  ## [1] 1.812994e-12

#model matrix
X= matrix(0, nrow=num_games, ncol=num_teams) for (m in 1:num_games) {
  X[m, 1] = 1
  home = table$Home[m]; if (home != 1) X[m, home] = 1 away = table$Away[m]; if (away != 1) X[m, away] = -1
}

model = glm.fit(X, table$Y, family=binomial()) data.frame(label=c("intercept", teams[-1,1]),
                                                          optim=result$par,
                                                          glm=model$coefficients)
#Standard error estimate
set.seed(2016)
x = (1:100) / 100 sigma = (1:100) / 100 B = 10000
estimates = numeric(B) default_se = numeric(B) robust_se = numeric(B)
get_robust_se = function(x, Y, estimate) { sqrt(sum(x^2 * (Y - estimate * x)^2)) / sum(x^2)
}

for(i in 1:B) {
  Y = x + rnorm(n=100, mean=0, sd=sigma)
  model = lm(Y ~ x + 0)
  estimates[i] = summary(model)[["coefficients"]][["x","Estimate"]] default_se[i] = summary(model)[["coefficients"]][["x","Std. Error"]] robust_se[i] = get_robust_se(x, Y, estimates[i])
}

true_se = sd(estimates) print(true_se)
## [1] 0.1326056
hist(robust_se, col=rgb(0, 0, 1, 0.3), breaks=20, xlim=c(0.06, 0.2), ylim=c(0, 2500),
     xlab="standard error", ylab="count",
     main="standard errors robust to heteroscedasticity")
hist(default_se, col=rgb(1, 0, 0, 0.3), breaks=20, add=TRUE) abline(v=true_se, col="blue", lwd=5)