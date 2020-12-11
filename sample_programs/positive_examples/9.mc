Main{
	
	// assuming that both matrices are of size nxm
	// n, m <= 1000

	int a[1004][1004], b[1004][1004], n, m, i, j;
	input -> n -> m;
	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < m; j = j + 1){
			input -> a[i][j];
		}
	}
	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < m; j = j + 1){
			input -> b[i][j];
		}
	}

	for(i = 0; i < n; i = i + 1){
		for(j = 0; j < m; j = j + 1){
			a[i][j] = a[i][j] + b[i][j];
			output <- a[i][j] <- ' ';
		}
		output <- '\n';
	}

}