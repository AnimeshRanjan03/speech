#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>
#include<conio.h>

using namespace std;

int Energy(int);
int dcshift();
int startmarker(double *,int,double);
void cal_ri(int,int);
void cal_ci(double);
void durbin();
void toukra_dis(double []);
int last,n=0;
double r[13];
double sample[100000];
double sine[]={2.553524963,4.001095013,5.243981669,6.19741645,6.796372338,6.999998801,6.794407923,6.193621598,5.2386152,3.994522936,2.546195513,0.992413066};
double tokhura_wt[]={0.0, 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0,61.0};
double c[13];
double finalci[5][13];
double avg_ci[13];


int dcshift()
{
	ifstream fp1;
	int sum=0,avg,i=0,b;
	fp1.open("Noise.txt");
	while(fp1>>b)
	{
		sum+=b;
		i++;
	}
	avg=sum/i;
	fp1.close();
	return avg;
}


int Energy(int count)
{
	double frame_energy[1000],max;
	double energy=0;
	int i,k=0,j,max_energy;
	double avg_energy=0;
	for(i=0;i<count;i++)
	{
		for(j=i;j<i+320;j++)
		{
			energy+=(sample[j]*sample[j]);

		}
		energy=energy/320.0;
		frame_energy[k]=energy;
		i=j;
		energy=0;
		k++;
	}

	for(i=0;i<k-1;i++)
	{
		avg_energy+=frame_energy[i];
	}
	avg_energy=(avg_energy/k);


	 int start=startmarker(frame_energy,k,avg_energy);
	
	max=frame_energy[start];
	for(i=start+1;i<last;i++)
	{
		if(max<frame_energy[i])
		{
			max=frame_energy[i];
			max_energy=i;
		}
		
	}

	return max_energy;
}


int startmarker(double *frame_energy,int k, double avg_energy)
{
	int i,start;

	for(i=1;i<k-1;i++)
	{
		if((frame_energy[i]-frame_energy[i-1])>(0.2*avg_energy))
		{
			start=i;
			i=k;
		}
	}
	for(i=k-2;i>0;i--)
	{
		if((frame_energy[i]-frame_energy[i-1])>(0.2*avg_energy))
		{
			last=i-1;
			i=0;
		}
	}

	return start;
}


void cal_ri(int start,int last)
{
	int i=0,j,k;
	double sum=0;

	 for(k=start*320;k<(last+1)*320;)
	{
	for(i=0;i<=12;i++)	
	{
	for(j=k;j<(k+320)-i;j++)
	{
		
		sum+=sample[j]*sample[j+i];		
	}
	    r[i]=sum;
		sum=0;
	}
	durbin();
	k=k+320;
	}


}


void cal_ci(double a[])
{
	int i,j;
	double sum=0;
	double k;
	
	for(i=1;i<=12;i++)
	{
		for(j=1;j<i;j++)
		{
			k=double(j)/double(i);
			sum += k*c[j]*a[i-j];
		}
		
		c[i]=a[i] + sum;
		sum=0;
	}
	for(i=1;i<=12;i++)
	{
		finalci[n][i]=(c[i]*sine[i-1]);
		
	}
	n++;
}



void durbin()
{
	int i,j;
	double sum=0;
	double e[13],k[13],a[13][13];

	
	e[0]=r[0];
	
	for(i=1;i<=12;i++)
	{
		for(j=1;j<i;j++)
		{
			sum+=a[j][i-1]*r[i-j];
		}

		
		k[i]=(r[i]-sum)/e[i-1];

		
		a[i][i]=k[i];
		for(j=1;j<i;j++)
		{
			a[j][i]=a[j][i-1]-(k[i]*a[i-j][i-1]);
		}

		
		e[i]=(1-(k[i]*k[i]))*e[i-1];
		sum=0;

	}

	double A[13];
	for(i=1;i<=12;i++)
	{
			A[i]=a[i][12];
		
	}
	
	cal_ci(A);
	
}

void toukra_dis(double avg_ci[])
{
	double a[5][13],data;
	ifstream atest;
	int i,j,k=0;
	double dis=0;
	double dist[25];
	int final;

	atest.open("dump_a.txt");
	for(i=0;i<5;i++)
	{
		for(j=1;j<=12;j++)
		{
			atest>>data;
			a[i][j]=data;

		}
	}
	double e[5][13];
	ifstream etest;
	etest.open("dump_e.txt");
	for(i=0;i<5;i++)
	{
		for(j=1;j<=12;j++)
		{
			etest>>data;
			e[i][j]=data;

		}
	}
	double ik[5][13];
	ifstream itest;
	itest.open("dump_i.txt");
	for(i=0;i<5;i++)
	{
		for(j=1;j<=12;j++)
		{
			itest>>data;
			ik[i][j]=data;

		}
	}
	double o[5][13];
	ifstream otest;
	otest.open("dump_o.txt");
	for(i=0;i<5;i++)
	{
		for(j=1;j<=12;j++)
		{
			otest>>data;
			o[i][j]=data;
		}
	}
	double u[5][13];
	ifstream utest;
	utest.open("dump_u.txt");
	for(i=0;i<5;i++)
	{
		for(j=1;j<=12;j++)
		{
			utest>>data;
			u[i][j]=data;
		}
	}
	for(i=0;i<5;i++)
	{
	for(j=1;j<=12;j++)
	{
		dis+=tokhura_wt[j]*(avg_ci[j]-a[i][j])*(avg_ci[j]-a[i][j]);
	}
	dist[k]=dis;
	dis=0;
	k++;
	}
	for(i=0;i<5;i++)
	{
	for(j=1;j<=12;j++)
	{
		dis+=tokhura_wt[j]*(avg_ci[j]-e[i][j])*(avg_ci[j]-e[i][j]);
	}
	dist[k]=dis;
	dis=0;
	k++;
	}
	for(i=0;i<5;i++)
	{
	for(j=1;j<=12;j++)
	{
		dis+=tokhura_wt[j]*(avg_ci[j]-ik[i][j])*(avg_ci[i]-ik[j][j]);
	}
	dist[k]=dis;
	dis=0;
	k++;
	}
	for(i=0;i<5;i++)
	{
	for(j=1;j<=12;j++)
	{
		dis+=tokhura_wt[j]*(avg_ci[j]-o[i][j])*(avg_ci[j]-o[i][j]);
	}
	dist[k]=dis;
	dis=0;
	k++;
	}
	for(i=0;i<5;i++)
	{
	for(j=1;j<=12;j++)
	{
		dis+=tokhura_wt[j]*(avg_ci[j]-u[i][j])*(avg_ci[j]-u[i][j]);
	}
	dist[k]=dis;
	dis=0;
	k++;
	}
	double min;
	min=dist[0];
	for(i=1;i<k;i++)
	{
		if(min>dist[i])
		{
			min=dist[i];
			final=i;
		}
	}
	if((final>=0)&&(final<5))
	{
		cout<<"Input Vowel is 'A' "<<endl;
	}
	if((final>=5)&&(final<10))
	{
		cout<<"Input Vowel is 'E' "<<endl;
	}
	if((final>=10)&&(final<15))
	{
		cout<<"Input Vowel is 'I' "<<endl;
	}
	if((final>=15)&&(final<20))
	{
		cout<<"Input Vowel is 'O' "<<endl;
	}
	if((final>=20)&&(final<25))
	{
		cout<<"Input Vowel is 'U' "<<endl;
	}

}


int main()
{
	int num;
	cout<<"Enter Number OF Files : "<<endl;
	cin>>num;
	ofstream obj("Testing.txt");
	
	while(num>0)
	{
		cout<<"Enter the file name"<<endl;
	
	ifstream input_file;

	
	
	string file_name;
	cin>>file_name;
	input_file.open(file_name);
	/*system("Recording_Module.exe 3 input_file.wav input_file.txt");
	fp.open("input_file.txt");*/
	int count=0,i=0,j,b;
	double s,sum=0;
	
	if(input_file.fail())
	{
		cout<<"File not found";
		return(1);
	}

	
	while(input_file>>s){
		sample[count]=s;
		count++;
	}

	
	input_file.close();

	int avg=dcshift();						//Calling DC Shift function

	
	for(i=0;i<count;i++)
	{
		sample[i]-=avg;						//Subtract the DCShift from the Input Sample.
	}
	
	
    double max=sample[0],min=sample[0];						/* MIN and MAX values
															for normalization*/
	for(i=1;i<count;i++)
	{
		if(max<sample[i])
		{
			max=sample[i];
		}
	}
	for(i=1;i<count;i++)
	{
		if(min>sample[i])
		{
			min=sample[i];
		}
	}

	 
	min=0-min;
 
	if(max<min)
	{
		max=min;
	}
	for(i=0;i<count;i++)
	{
		sample[i]=(sample[i]/max)*5000;
	}
	int start=Energy(count);
	last=start+2;
	start=start-2;
	cal_ri(start,last);

	for(i=1;i<=12;i++)
	{
		for(j=0;j<5;j++)
		{
			sum+=finalci[j][i];
		}
		avg_ci[i]=sum/5;
		sum=0;
	}
	  
	num--;
	n=0;
	toukra_dis(avg_ci);
	}

	getch();
	return 0;
}
