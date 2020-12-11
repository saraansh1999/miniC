Main{
	int x, y, z, n, m, cnt;
	z = 0;
	m = 2;
	cnt = 0;
	while(z <= 1000){
		for(n = 1; n < m; n = n + 1){
			x = m*m - n*n;
			y = 2*m*n;
			z = m*m + n*n;

			if(z > 1000){
				break;
			}

			output <- x <- ' ' <- y <- ' ' <- z <- '\n';
			cnt = cnt + 1;
		}
		m = m + 1;
	}
	output <- cnt <- '\n';
}