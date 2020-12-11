Main{

	int n, arr[1000005], i;
	input -> n;
	for(i = 0; i < n; i = i + 1){
		input -> arr[i];
	}

	int pos, cnt[10], out[1000005], ind;
	pos = 1;
	bool sorted;
	sorted = false;
	while(!sorted){
		sorted = true;
		
		for(i = 0; i < 10; i = i + 1){
			cnt[i] = 0;
		}

		for(i = 0; i < n; i = i + 1){
			ind = (arr[i]/pos)%10;
			cnt[ind] = cnt[ind] + 1;
		}

		for(i = 1; i < 10; i = i + 1){
			cnt[i] = cnt[i - 1] + cnt[i];
		}

		for(i = n-1; i >= 0; i = i - 1){
			ind = (arr[i]/pos)%10;
			out[cnt[ind] - 1] = arr[i];
			cnt[ind] = cnt[ind] - 1;
		}


		for(i = 0; i < n; i = i + 1){
			arr[i] = out[i];
			if(i > 0 && arr[i] < arr[i-1]){
				sorted = false;
			}
		}		

		pos = pos * 10;
	}
	for(i = 0; i < n; i = i + 1){
		output <- arr[i] <- ' ';
	}
	output <- '\n';

}