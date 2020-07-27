//  *********************************************************************
//  *                   This file is part of RES 6.0.                   *
//  *    RES 6.0 is an original software distributed within the book    *
//  *                                                                   *
//  *      |-----------------------------------------------------|      *
//  *      | "Speech Recognition: Theory and C++ Implementation" |      *
//  *      |               John Wiley & Sons, Ltd                |      *
//  *      |    by Claudio Becchetti and Lucio Prina  Ricotti    |      *
//  *      |-----------------------------------------------------|      *
//  *                                                                   *
//  *       See copyright.txt file for further info. on copyright       *
//  *********************************************************************

//  _____________________________________________________________________
//  |-------------------------------------------------------------------|
//  |                                                                   |
//  |   FILE:   dspproc.cpp                                             |
//  |   FUNCTIONALITY: low level C-like procedure for signal processin  |
//  |   PROGRAM: required to feature extraction                         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19//11/98                                 |
//  |   FIRST VERSION:   1.0     (newiofile.cpp)                        |
//  |   DATA FIRST VERSION: 17/01/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
#include "../features/feature.h"
//************user info**************

// PROTOTYPE OF C FUNCTIONS	*************************************************************

	void xft(t_real *vr,t_real *vi,t_signed order,t_signed s);

	void shuffle(t_real  *vr,t_real  *vi,t_signed order);

	t_index Order_Log2(t_index num);

	void mfcc(t_real  *dp_fft,  t_real  *dp_mfcc, t_real d_sam, t_signed l_nfft, t_signed l_nmfcc);

// ***************************************************** END ******


// fft: keeps  preemphasized vector of time samples (variable vet_re)
// and returns the real (vet_re) and the immaginary (vet_im) parts of its fft
void Fft_Call(VetDouble & vet_re, VetDouble & vet_im)
	{
	t_signed one=1L,lx,i,order;
	t_real x;
	t_real * vr;
	t_real *vi;

	order = Order_Log2(vet_re.Dim());
	
	vr=&(vet_re[0]);
	vr=vr-1;

	Assert(&(vr[1])==(t_real   *)(&(vet_re[0])));

	vet_im.Destroy_And_ReDim(vet_re.Dim());
	vi=&(vet_im[0]);
	vi=vi-1;

	xft((t_real * )(vr),(t_real * )(vi),order,one);

	x  = (t_real)(pow(2.0,(t_real)order));
	lx = (t_signed)x;
	x  = (t_real)sqrt( (t_real)x);

	for(i=1L;i<=lx;i++)
		{       
		*(vr+i) = *(vr+i) / x;
		*(vi+i) = *(vi+i) / x;
		}
 	
	return;
	}

// mfcc_call: 
void Mfcc_Call(VetDouble & mfc_vet, VetDouble & fft_module, 							t_real d_sam)
	{
	t_real   * dp_fft;
	t_real  *df_mfcc;
	t_signed l_nfft,l_nmfcc;

	l_nfft=fft_module.Dim();
	l_nmfcc=mfc_vet.Dim();
	df_mfcc=&(mfc_vet[0]);
	df_mfcc=df_mfcc-1;

	Assert(&(df_mfcc[1])==(t_real *)(&(mfc_vet[0])));
	dp_fft=&(fft_module[0]);
	dp_fft=dp_fft-1;
	mfcc(dp_fft,df_mfcc,d_sam,l_nfft,l_nmfcc);
	return;
	}



// ifft: inverse FFT transform. Gets a vector of DFT samples in vet_re 
// and returns the inverse transform in vet_re and vet_im
// Attention: the form of the input array must be specified
void Ifft_Call(VetDouble & vet_re, VetDouble & vet_im)
	{   
	t_signed nega= -1L,order;
	t_real x;
	t_signed i,lx;
	t_real   * vr;
	t_real  *vi;

	order = Order_Log2(vet_re.Dim());

	vr=&(vet_re[0]);
	vr=vr-1;

	//this Assert assures that the operation has been done correctly

	Assert(&(vr[1])==(t_real *)(&(vet_re[0])));

	vet_im.Destroy_And_ReDim(vet_re.Dim());
	vi=&(vet_im[0]);
	vi=vi-1;

	x   =  (t_real)(pow(2.0,(t_real)order));
	lx  =  (t_signed)x;
	x   =  (t_real)sqrt( (t_real)x);

	xft(vr, vi,order,nega);
	for(i=1L;i<=lx;i++)
		{
		*(vr+i) = *(vr+i) / x;
		*(vi+i) = *(vi+i) / x;
		}

	return;
	}



t_index Order_Log2(t_index num)
	{
	t_index act_num,order;
	order=0;

	do 	{
		order++;
		act_num= 1L<<order;
		if (act_num>num)
			merr<<"FFT of not 2 power dimension required";
			//return -act_num;
		} while(act_num!= num);
	return order;
	}


/*

 ****************************************************************************
 * given the complex vector with real part xr[1..n] and imaginary part,      *
 * xi[1..n], computes the FFT on NN=2^order points and puts results in the   *
 * xr[1..NN], xi[1..NN] vector position, if SIGN>=0                          *
 * computes the reverse FT if SIGN is negative                                *
 * the routine SHUFFLE is referenced.                                        *
 *****************************************************************************

*/

void xft(t_real   * vr,t_real  *vi,t_signed order, t_signed s)
	{
	register int i,j,n,n2;
	int z,k,i_or;
	t_real wr,wi,arg,dwr,dwi,tr,ti,theta;

	if (s >= 0)   theta =  (t_real)acos( -1.0 );
	else          theta=   (t_real)(-1.0*acos( -1.0));

	shuffle(vr,vi,order);
	n2=1;
	i_or = (int)order;
	for (i=1; i<=i_or; i++)
		{
		n=n2;
		n2 += n2;
		wr= (t_real)1.0;
		wi= (t_real)0.0;
		arg=theta/(t_real)n;
		dwr= (t_real)cos( (t_real)arg);
		dwi= (t_real)sin( (t_real)arg);
		for (j=1; j<=n; j++)
			{
			for (k=j; k<=(1<<i_or); k+=n2) 
				{
				z = k + n;
				tr = vr[z]*wr + vi[z]*wi;
				ti = vi[z]*wr - vr[z]*wi;
				vr[z] = vr[k] - tr;
				vi[z] = vi[k] - ti;
				vr[k] += tr;
				vi[k] += ti;
				}
			tr = wr*dwr - wi*dwi;
			wi = wi*dwr + wr*dwi;
			wr = tr;
			}
		}
	return;
	}

void shuffle(t_real  *vr,t_real  *vi,t_signed order)
	{
	int i,j,k,n2,i_or;
	t_real t;

	i_or = (int)order;
	n2 = (1<<i_or)>>1;
	j=1;
	for (i=1; i<(1<<i_or); i++) 
		{
		if (i<j)
			{
			t     =  vr[i];
			vr[i] =  vr[j];
			vr[j] =  t;
			t     =  vi[i];
			vi[i] =  vi[j];
			vi[j] =  t;
			}
		k = n2;
		while (k<j)
			{
			j -= k;
			k >>= 1;
			}
		j += k;
		}
	return;
	}


/*
 ****************************************************************************
 * ------------------------------------------------------------------------- *
 * REFERENCES                                                                *
 * Davis, Mermelstein "Comparison of parametric representation ..."          *
 * ASSP Vol.28, No.4, August 80, pp.357                                      *
 *****************************************************************************
*/

void mfcc(t_real  *dp_fft,  t_real  *dp_mfcc, t_real d_sam,
	  t_signed l_nfft, t_signed l_nmfcc)
	{
	static String buffer;
	t_real band,rowamp,onek;
	t_real  *filterbank;
	t_signed i,j,n,k,nfilterbank;
	t_signed delta,left,center,rigth;
	t_real a,b,pigr;

	t_real zero = 0.0;
	t_real fdelta;
	t_real factor=1.2;

	band   = d_sam/2.0;
	rowamp = band/(t_real)l_nfft;

	if( l_nfft!=64L && l_nfft!=128L && l_nfft!=256L && l_nfft!=512L && l_nfft!=1024)
		{
		merr<<"\nIN MFCC <l_nfft> IS"<<l_nfft
			 <<"overflow in mfcc coefficient";
		}

	if ((sizeof(t_real)* (size_t)( l_nfft+4L ))!=buffer.Dim())
		buffer.Destroy_And_ReDim(sizeof(t_real)* (size_t)( l_nfft+4L ));

	filterbank=(t_real *)&(buffer[0]);
	
	for(i=0L;i<=l_nfft;i++)
			filterbank[i]=zero;
/*
	********************************************************
	*                                                      *
	*   INITIALIZE FILTER DEFINITION IN LINEAR PART        *
	*                                                      *
	********************************************************
*/
	i       = 1L;
	onek    = 1000.0/rowamp;    /* rows in 1kHz */
	fdelta  = onek/10.0;        /* we wants about 10 cep uder 1kHz */
	delta   = (t_signed)fdelta;
	n       = 1L;
	left    = n;
	center  = n + delta;
	rigth   = n + 2L*delta;
/*
	********************************************************
	*                                                      *
	*   NOW COMPUTES THE LINEAR SCALE BANKFILTER  f<1kHz   *
	*                                                      *
	********************************************************
*/
	/* until the central boundary is lower than 1kHz */
	while(n < (t_signed)onek )
		{
		filterbank[i] = 0.0;
		/* compute the decrement step unit for triangular filters */
		b = 2.0 + (t_real)rigth - (t_real)left;
		b = (b-1.0)/2.0;
		b = 1.0/b;
		/* b is the step unit */
		for(j=left; j<=rigth; j++ ) 
			{
			/* a is the point distance from the centre of the
			   triangular filter
			*/
			a = fabs( (t_real)center - (t_real)j);
			a = 1.0 - a*b;
			/* a is the factor<1 to amplify row(j) */
			/* update filter i in filterbank computation */
			filterbank[i] = filterbank[i] + dp_fft[j]*a;
			}
		/* NOW
		   1) compute next filter i -> i+1
		   2) delta is still the same (linear grows)
		   3) filter center moves up of delta points
		   4) left and rigth boundary are updated
		*/

		i     = i +1L;
		n      = n + delta;
		left   = n;
		center = n + delta;
		rigth  = n + 2L*delta;
		}


//	********************************************************
//	*                                                      *
//	*   NOW COMPUTES THE LOG SCALE BANKFILTER  f>1kHz      *
//	*                                                      *
//	********************************************************

	// until the rigth boundary is inside the FFT 
	while(rigth < l_nfft ) 
		{
		filterbank[i] = 0.0;

		/* compute the decrement step unit for triangular filters */
		b = 2.0 + (t_real)rigth - (t_real)left;
		b = (b-1.0)/2.0;
		b = 1.0/b;
		/* b is the step unit */
		for(j=left; j<=rigth; j++ ) 
			{
			/* a is the point distance from the centre of the
			   triangular filter
			*/
			a = fabs( (t_real)center - (t_real)j);
			a = 1.0 - a*b;
			// a is the factor<1 to amplify row(j) 
			// update filter i in filterbank computation
			filterbank[i] = filterbank[i] + dp_fft[j]*a;
			}
		/* 
		
		NOW
		   1) compute next filter i -> i+1
		   2) delta grows exponentially delta -> delta*factor
		   3) filter center moves up of delta points
		   4) left and rigth boundary are updated
		
		*/

		i     = i +1L;
		fdelta = factor*fdelta;
		delta = (t_signed) fdelta;
		n      = n + delta;
		left   = n;
		center = n + delta;
		rigth  = n + 2L*delta;
		}


//	********************************************************
//	*                                                      *
//	*   LOG ENERGY FILTERBANK AND CEPSTRUM COMPUTATION     *
//	*                                                      *
//	********************************************************

	nfilterbank = i-1;                /* the number of filter */
	if( nfilterbank < l_nmfcc ) 
		merr<<"\nIN MFCC <nfilter> IS "<<nfilterbank<<" AND <nmfcc> IS "<<l_nmfcc<<endl;
		

	// LOG computation of the filterbank,
	//   no negative values are allowed, if E<1 => E=1.
	for(i=1;i<=nfilterbank;i++) 
		{
		if( filterbank[i] >= 1.0 ) 
			{
			filterbank[i] = 20.0*log10( filterbank[i] );
			} else  {
					filterbank[i] = 0.0;
					}
		}
	/* the cosine transformation to obtain MFCC */
	pigr = acos( -1.0);
	for(i=1;i<=l_nmfcc;i++) 
		{
		dp_mfcc[i] = 0.0;
		for(k=1;k<=nfilterbank;k++) 
			{
			a = (t_real)k - 0.5;
			a = a * pigr * (t_real)i;
			a = a/(t_real)nfilterbank;
			a = cos(a);
			dp_mfcc[i] += filterbank[k] * a;
			}
		}
	//free( (char *)filterbank);
	return;
	}
