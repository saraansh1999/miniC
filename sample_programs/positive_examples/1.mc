Main{
	int power(int i, int k){
		int prod;
		prod = 1;
		while(k > 0){
			prod = prod * i;
			k = k - 1;
		}
		return prod;
	}
	int N, k, ans, i;
	input -> N -> k;
	i = 1;
	ans = 0;
	while(i <= N){
		ans = ans + power(i, k);
		i = i + 1;
	}
	output <- ans <- '\n';
}
