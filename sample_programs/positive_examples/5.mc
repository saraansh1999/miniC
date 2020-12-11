Main{
	
	int arr[1000005];
	bool vis[1000005];

	int printer(int n){
		int i;
		for(i = 1; i <= n; i = i + 1){
			if(vis[i]){
				output <- i <- " ";
			}
		}
		output <- '\n';
		return 0;
	}

	int combinations(int n, int i){
		if(i > n){
			printer(n);
			return 0;
		}
		combinations(n, i+1);
		vis[i] = true;
		combinations(n, i+1);
		vis[i] = false;
		return 0;
	}

	int n, i;
	input -> n;
	for(i = 1; i <= n; i = i + 1){
		arr[i] = i;
		vis[i] = false;
	}
	combinations(n, 1);
}