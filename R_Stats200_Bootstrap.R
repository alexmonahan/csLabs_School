X = c(3, 0, 2, 1, 3, 2, 1, 0, 2, 1);
n = length(X)
B=10000
nonparam = numeric(B)
for (i in 1:B) {
  X_star = sample(X, size=n, replace=TRUE)
  a_1 = length(which(X_star==1))
  a_1 = a_1 + length(which(X_star==0))
  a_2 = 10-a_1
  test_stat = (a_1/10)
  nonparam[i] = test_stat
}
print(sd(nonparam))