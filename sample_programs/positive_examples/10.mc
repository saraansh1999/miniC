Main{
	
	// assuming that both matrices are of size nxn
	// n, m <= 1000
	// assuming dot product is required

	int a[1004][1004], b[1004][1004], c[1004][1004], n, i, j, k, sum;
	input -> n;
	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < n; j = j + 1){
			input -> a[i][j];
		}
	}
	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < n; j = j + 1){
			input -> b[i][j];
		}
	}

	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < n; j = j + 1){
			sum = 0;
			for(k = 0; k < n; k = k + 1){
				sum = sum + a[i][k] * b[k][j];
			}
			c[i][j] = sum;
			output <- c[i][j] <- ' ';
		}
		output <- '\n';
	}

}