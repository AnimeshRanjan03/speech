// Yes_No.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<string>
#include<conio.h>
#include<stdlib.h>

using namespace std;

///////////////////////////////////-----Function TO Calculate DC Shift By taking input from Ambience Noise-----//////////////////////////////////////////////////

	long double	dc,
				sample[100000],
				maxAmp,
			
				zcr[1000],
				energy[1000],
				ener,
				z=0,
				avgZCR,
				avgEnergy;

	long frame_count, 
		 count_sample,
		 k,
		 zc,
		 start,
		 endMarker;

///////////////////////////////////-----Function TO Calculate Absolute Value-----//////////////////////////////////////////////////
int dc_shift()
{
	ifstream inputNoise;										/*Object of File*/
	int sum=0,count=0,dc=0,a;

	inputNoise.open("Noise.txt");								/*Opening file Noise.txt*/
	while(inputNoise>>a)
	{
		sum=sum+a;
		count++;
	}
	inputNoise.close();											/*Closing function of Noise File file*/
	dc=sum/count; 
	return dc;
	
}
///////////////////////////////////-----Function TO Calculate Absolute Value-----//////////////////////////////////////////////////
void absolute()
{
	for(int i=0;i<count_sample;i++)
	{
			long double max=abs(sample[i]);						/*Calculating absolute value for getting maximum pitch of the speech signal*/
		if(max>=maxAmp)
		{
			maxAmp=max;
		}
	}
	
}
///////////////////////////////////-----Function TO Normalize the Input Samples-----//////////////////////////////////////////////////
void normalize()
{
	for(int i=0;i<count_sample;i++)
	{
		sample[i]=(sample[i]/maxAmp)*5000;						/*Normalization of samples*/
	}
}
///////////////////////////////////-----Function TO Calculate Calculate and ZCR by frames-----//////////////////////////////////////////////////
void calEnergyandZCR()
{
	long i,j;
	for(i=0;i<count_sample;i++)
	{
		for(j=i;j<i+100;j++)
		{
			ener+=(sample[j]*sample[j]);						/*Calculating squares of sample to fing ENERGY*/
			if((sample[j]*sample[j+1])<0||sample[j]==0)			/*Calculating ZCR by : Checking change in sign of consecutive sample if it changes or Touches ZERO*/
			{
				z=z+1;
			}

		}
		ener=ener/100;											/*Final Energy for particular chunck of 100 samples */
		zcr[zc++]=z;
		z=0;
		energy[k++]=ener;
		ener=0;
		i=j;
	}

}
///////////////////////////////////-----Function TO Calculate Average Energy and ZCR-----//////////////////////////////////////////////////
void avgEnerAndZCR()
{
	for(long l=0;l<frame_count;l++)
	{
		//cout<<"Energy of "<<l+1<<" chunck is :"<<energy[l]<<"  and      ZCR : "<<zcr[l]<<endl;
		avgEnergy+=energy[l];
		avgZCR+=zcr[l];
	}
	avgZCR=avgZCR/frame_count;
	avgEnergy=avgEnergy/frame_count;							/*Calculating Average ZCR of whole input sample*/
	cout<<"Average ZCR :"<<avgZCR<<endl;
	cout<<"Average Energy : "<<avgEnergy<<endl;

}
///////////////////////////////////-----Function TO Put Start and end markers in correct positions-----//////////////////////////////////////////////////
void StartAndEnd()
{
	bool flag=false;
	for(long m=0;m<frame_count;m++)
	{
		if((energy[m]-energy[m-1])>.2*avgEnergy&&flag==false) //Putting start marker to recognise the start of actuall speech signal
															  //i.e. by checking abrupt change in energy of 2 chuncks
		{
			start=m;
			flag=true;
			break;
		}
	}
	for(long i=frame_count-1;i>0;i--)
	{
		if(flag==true&&(energy[i]-energy[i-1]>.2*avgEnergy))
		{
			endMarker=i;
			flag=false;
			break;
		}
	}
}
///////////////////////////////////-----Function TO Recognise the Type of Speech-----//////////////////////////////////////////////////
void testSpeech()
{
	if(avgZCR>=20)												/*Deciding by manual analysis that what is behaviour of YES and NO speech signals
																and checking what is input signal*/
	{
		cout<<"Input Speech is YES"<<endl;
	}
	else{ 
		
		cout<<"Input Speech is NO"<<endl;

	
	}
	
}

int main()
{	
	system("Recording_Module.exe 3 input.wav input.txt");	
	
	ifstream inputYes;
	inputYes.open("input.txt");
	float s;

	while(inputYes>>s)
	{		
		sample[count_sample]=s;
		sample[count_sample]-=dc;								/*Subtracting DC shift form whole sample*/
		//cout<<sample[count_sample]<<endl;
		count_sample++;
		
	}
	inputYes.close();
	
	frame_count=count_sample/100;								/*Frame count to store ZCR and Energy for diff. sample chunks*/
	cout<<"DC Shift : "<<dc<<endl;
	cout<<"No of sample : "<<count_sample<<endl;

	///////////////////////////////////------------------->>>CALLING ALL FUNTIONS<<<-------------------/////////////////////////////////////////////
	dc=dc_shift();											/*Calling DC shift fuction*/
	absolute();												
	cout<<"Max Amplitude : "<<maxAmp<<endl;
	normalize();
	calEnergyandZCR();
	avgEnerAndZCR();
	StartAndEnd();
	cout<<"Start : "<<start<<endl;
	cout<<"End : "<<endMarker<<endl;
	testSpeech();
	getch();
}

