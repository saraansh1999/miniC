Main{
	
	bool is_prime(int N){
		int i;
		bool isprime;
		isprime = true;
		if(N == 0 || N == 1){
			isprime = false;
		}
		else{
			for(i = 2; i*i <= N; i = i + 1){
				if(N%i == 0){
					isprime = false;
					break;
				}
			}
		}
		return isprime;
	}

	int N, ans, i;
	ans = 0;
	input -> N;
	for(i = 0; i < N; i = i + 1){
		if(is_prime(i)){
			ans = ans + i;
		}
	}
	output <- ans <- '\n';
}