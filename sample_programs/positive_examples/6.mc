Main{
	int n, arr[1000005], i, j, k;
	input -> n;
	for(i = 0; i < n; i = i + 1){
		input -> arr[i];
	}

	for(i = 1; i < n; i = i + 1){
		k = arr[i];
		j = i - 1;
		while(j >= 0 && arr[j] > k){
			arr[j+1] = arr[j];
			j = j - 1;
		}
		arr[j+1] = k;
	}

	for(i = 0; i < n; i = i + 1){
		output <- arr[i] <- ' ';
	}
	output <- '\n';
}