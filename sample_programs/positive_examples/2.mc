Main{
	int N, i;
	bool isprime;
	isprime = true;
	input -> N;
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
	output <- isprime <- '\n';
}