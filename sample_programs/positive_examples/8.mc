Main{
	
	int arr[1000005];

	int merge(int s, int m, int e){
		int temp[1000005], p1, p2, c;
		p1 = s;
		p2 = m+1;
		c = 0;

		while(p1 <= m && p2 <= e){
			if(arr[p1] < arr[p2]){
				temp[c] = arr[p1];
				p1 = p1 + 1;
			}
			else{
				temp[c] = arr[p2];
				p2 = p2 + 1;
			}
			c = c + 1;
		}

		while(p1 <= m){
			temp[c] = arr[p1];
			p1 = p1 + 1;
			c = c + 1;
		}

		while(p2 <= e){
			temp[c] = arr[p2];
			p2 = p2 + 1;
			c = c + 1;
		}

		for(c = s; c <= e; c = c + 1){
			arr[c] = temp[c-s];
		}

		return 0;
	}

	int mergesort(int s, int e){
		if(s == e){
			return 0;
		}
		int m;
		m = (s+e)/2;
		mergesort(s, m);
		mergesort(m+1, e);
		merge(s, m, e);

		return 0;
	}

	int n, i;
	input -> n;
	for(i = 0; i < n; i = i + 1){
		input -> arr[i];
	}

	mergesort(0, n-1);

	for(i = 0; i < n; i = i + 1){
		output <- arr[i] <- ' ';
	}
	output <- '\n';
}