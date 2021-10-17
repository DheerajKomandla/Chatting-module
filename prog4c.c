#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int crdd(int ck){
	int count=0;
	printf("ck",ck);
if(ck>26)
	{
		count++;
		printf("The maximum number of routers supported is 26\n");	
		return 0;
	}
	return count;
}
int main()
{
	char ffle[50]; //fle name
	int counnt; //no of ruters
	int flagvrble[200]; //visted or not
	int lstnde[200]; //lst nde
	int cst[200]; //cost of pth
	int mnmm; //mnm vale
	char sorcee;//surce cde
	char fistt; //fdt nde
	FILE *pnter; //pointer
	//varbless
	int ch=0;  
	int hs=0;
	int shh=0;
	int ch1;
	int hr;
	int md=0;
	
	
	printf("OSPF Link-State (LS) Routing:");
	printf("\n-------------------------------\n");
	
	//validating routers
	
	printf("Enter the number of routers:");
	scanf("%d",&counnt);
	
	int som=crdd(counnt);
	if (som!=0)
	{
		return 0;
	}
	
//cost values store
	
	char su[counnt];
	int us[counnt][counnt];
	
	//input from usr
	printf("Enter filename with cost matrix values:");
	scanf("%s",ffle);
	
		pnter=fopen(ffle,"r");//opng fle
	
	
	
	
	if(pnter==NULL)//valdn
	{
		printf("error: unable to open file:%s\n",ffle);
		return 0;
	}
	


	for(ch=0;ch<counnt;ch++)//vles to arry
	{
		for(hs=0;hs<counnt;hs++)
		{
			
			fscanf(pnter,"%d",&us[ch][hs]);
			
		}
	}
	
	
	
	for(ch=0;ch<counnt;ch++) //logic
	{
		for(hs=0;hs<counnt;hs++)
		{
			if(us[ch][hs]<0)
			{
				us[ch][hs]=999;
			}
		}
	}

	
	printf("Enter character representation of first node: ");//user input
    scanf(" %c",&fistt);
	
	
	printf("Enter the source router:");//src nde inpt
    scanf(" %c",&sorcee);
	
	
	if(fistt >= 'su' && fistt<='md' && sorcee>='su' && sorcee<='md')//valdting 
	{
		ch1=sorcee-fistt;
	}
	
	else
	{
		printf("Please give lower case letters to represent nodes\n");
		return 0;
	}
	
	/*OSPF link state alogrithm logic starts here*/
	ch=0;
	while(ch<counnt)
	{
		flagvrble[ch]=0;
		lstnde[ch]=ch1;
		cst[ch]=us[ch1][ch];
		ch++;
	}
	flagvrble[ch1]=1;
	ch=0;
	while(ch<counnt)
	{
		mnmm=999;
		for(shh=0;shh<counnt;shh++)
		{
			if(!flagvrble[shh])
				
				if(cst[shh]<mnmm)
				{		
					hr=shh;
					mnmm=cst[shh];
				}
		}
		
		flagvrble[hr]=1;
		
		for(shh=0;shh<counnt;shh++)
		{
			if(!flagvrble[shh])
				
				if(mnmm+us[hr][shh]<cst[shh])
				{
					cst[shh]=mnmm+us[hr][shh];
					lstnde[shh]=hr;
				
				}
		}
			ch++;
	}
	ch=0;
	
	while(ch<counnt)
	{	
		md=0;
		int vm;
		for(vm=0;vm<counnt;vm++)
		{
			su[vm]='\0';
		}
		printf("%c ==> %c",sorcee,fistt+ch);
		hr=ch;
		while(hr!=ch1)
		{
			su[md]=fistt+lstnde[hr];
			md++;
			hr=lstnde[hr];
		}
		
		printf("\npath cost: %d",cst[ch]);
		
		printf("\npath taken :");
		md--;
		
		while(md>=0)
		{
			printf(" %c --> ",su[md]);
			md--;
		}
		printf("%c\n",fistt+ch);
		ch++;

	}
    fclose(pnter); //close
    return 0;
}

			
			
	
	
		
				