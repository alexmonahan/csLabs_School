n = 500
B = 10000
lamb_hat = numeric(B)
se_Fisher = numeric(B)
se_sandwich = numeric(B)
for (i in 1:B) {
  X = rgamma(n,2,rate=1)
  lamb_hat[i] = 1/mean(X)
  se_Fisher[i] = 1/(mean(X)*sqrt(n))
  se_sandwich[i] = sd(X)/(mean(X)^2*sqrt(n))
}
print(mean(lamb_hat))
print(sd(lamb_hat))
hist(se_Fisher)
hist(se_sandwich)