X.median = numeric(5000)
for(i in 1:5000) {
  X = rnorm(99, mean = 0, sd = 1)
  X.median[i] = median(X)
}
print(mean(X.median))
print(sd(X.median))
hist(X.median)