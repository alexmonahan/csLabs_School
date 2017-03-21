gamma.MLE = function(X) { 
ahat = compute.ahat(X) bhat = ahat / mean(X)
return(c(ahat, bhat)) }

# estimate ahat by Newton-Raphson
compute.ahat = function(X) {
  a.prev = -Inf
  a = mean(X)^2 / var(X) # initial guess
  # while not converged, do Newton-Raphson update
  while(abs(a - a.prev) > 1e-12) {
    a.prev = a
    numerator = -f(a.prev) + log(mean(X)) - mean(log(X)) denominator = f.prime(a.prev)
    a = a.prev + numerator / denominator
  }
  return(a) }


f = function(alpha) { return(log(alpha) - digamma(alpha))
}
f.prime = function(alpha) {
  return(1 / alpha - trigamma(alpha))
}



#Run simulation
n = 500
n.reps = 5000
alpha = 1
beta = 2
alpha.hat = numeric(n.reps) beta.hat = numeric(n.reps)
for (i in 1:n.reps) {
  X = rgamma(n, shape = alpha, rate = beta) estimates = gamma.MLE(X)
  alpha.hat[i] = estimates[1]
  beta.hat[i] = estimates[2]
}


hist(alpha.hat, breaks=20)