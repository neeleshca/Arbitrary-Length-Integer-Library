
#include "intal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{

	int *arr;
	int len;
}arint;

#define MAX(a,b) (((a>b))?(a):(b))

static void* karatsuba(void* intal1, void* intal2);
static void* leading_zeroes(void* intal1);
static void* power_recursive(void* intal1,void* intal2);

void* intal_create(const char* str){


	if(str[0]<48 || str[0]>57){
		arint *temp = (arint *)malloc(sizeof(arint));
		temp->arr = (int *)malloc(sizeof(int));
		temp->len = 1;
		temp->arr[0]=0;
		return temp;
	}
	arint *temp = (arint *)malloc(sizeof(arint));
	temp->arr = (int *)malloc(strlen(str)*sizeof(int));
	temp->len = strlen(str);
	for(int i=0;i<strlen(str);i++)
		if(str[i]>=48 && str[i]<=57){
			temp->arr[i]=str[i]-'0';
		}
		else{
			temp->arr = (int *)realloc(temp->arr,i*sizeof(int));
			temp->len=i;
			temp=leading_zeroes(temp);
			return temp;
		}

	temp=leading_zeroes(temp);
	return temp;

}

void intal_destroy(void* intal){
		arint *temp = (arint *)intal;
		if(temp!=NULL){
			free(temp->arr);
			free(temp);
		}

}


char* intal2str(void* intal){

	if(intal==NULL){
		char *temp = malloc(4);
		temp[0]='N';
		temp[1]='a';
		temp[2]='N';
		temp[3]='\0';
		
		return temp;
	}
	arint *temp1 = (arint *)intal;
	//printf("Length of array is %d\n",temp1->len);
	char *temp = malloc(temp1->len+1);
	for(int i=0;i<temp1->len;i++){
		temp[i]=temp1->arr[i]+'0';
	}
	temp[temp1->len]='\0';


	return temp;
}

//A new object is always made for increment and decrement. For cases like intal1 = increment/decrement(intal3), intal3 should retain its original value.
//A new object is always made for increment and decrement. For cases like intal1 = increment/decrement(intal3), intal3 should retain its original value.
void* intal_increment(void* intal){

	arint *arr1=(arint *)intal;
	if(arr1==NULL)
		return NULL;
	char *temp = "1";
	void *intal2 = intal_create(temp);
	void *intal3 = intal;
	intal = intal_add(intal,intal2);
	intal_destroy(intal3);
	intal_destroy(intal2);
	return intal;
}

void* intal_decrement(void* intal){

	arint *arr1=(arint *)intal;
	if(arr1==NULL)
		return NULL;

	
	
	arint *need = (arint *)intal;
	if(need->len==1 && need->arr[0]==0)
		return intal;

	char *temp = "1";
	void *intal2 = intal_create(temp);

	void *intal3 = intal;
	intal = intal_diff(intal,intal2);
	intal_destroy(intal3);
	intal_destroy(intal2);
	return intal;
}


void* intal_add(void* intal1, void* intal2){


	arint *arr1=(arint *)intal1;
	arint *arr2=(arint *)intal2;

	if(arr1==NULL || arr2==NULL){
		return NULL;
	}
	if(arr1->len<arr2->len)
	{
		arint *temp = arr1;
		arr1=arr2;
		arr2=temp;
	}

	arint *temp = malloc(sizeof(arint));


	temp->arr = (int *)malloc((arr1->len+1)*sizeof(int));
	temp->len = arr1->len+1;


	int carry=0;
	int i=arr1->len-1;
	int j=arr2->len-1;
	int sum=0;
	//printf("%d\n%d\n",i,j);

	while(j>=0)
	{
		sum=arr1->arr[i]+arr2->arr[j] +carry;
		

		if(sum>=10)
		{
			carry = 1;
			sum-=10;
		}
		else
			carry = 0;
		temp->arr[i+1]=sum;
		//printf("Sum is %d   %d\n",i+1,sum);
		j--;
		i--;
	}

	while(i>=0)
	{
		sum=arr1->arr[i]+ 0 +carry;
		

		if(sum>=10)
		{
			carry = 1;
			sum-=10;
		}
		else
			carry = 0;
		temp->arr[i+1]=sum;
		//printf("Sum is %d   %d\n",i+1,sum);
		j--;
		i--;
	}
	if(carry==1)
	{
		temp->arr[0]=1;
		return temp;
	}
	else
	{	
		arint *temp1 = malloc(sizeof(arint));

		temp1->arr = (int *)malloc((arr1->len)*sizeof(int));
		temp1->len = arr1->len;

		for(int i=0;i<arr1->len;i++)
			temp1->arr[i]=temp->arr[i+1];

		intal_destroy(temp);
		return temp1;
	}

}

//Returns the difference (obviously, nonnegative) of two intals.
void* intal_diff(void* intal1, void* intal2){
	arint *arr1=(arint *)intal1;
	arint *arr2=(arint *)intal2;

	if(arr1==NULL || arr2==NULL){
		return NULL;
	}

	if(intal_compare(intal1,intal2) == -1)
	{
		arint *temp = arr1;
		arr1=arr2;
		arr2=temp;
	}

	arint *temp = malloc(sizeof(arint));

	temp->arr = (int *)malloc(arr1->len*sizeof(int));
	temp->len = arr1->len;

	

	int borrow=0;
	int i=arr1->len-1;
	int j=arr2->len-1;
	int diff1=0;
	//printf("%d\n%d\n",i,j);

	while(j>=0)
	{
		diff1=arr1->arr[i]-arr2->arr[j] - borrow;
		

		if(diff1<0)
		{
			borrow = 1;
			diff1+=10;
		}
		else{
			borrow  = 0;
		}
		temp->arr[i]=diff1;
		//printf("Diff is %d   %d\n",i,diff1);
		j--;
		i--;
	}
	while(i>=0)
	{

		diff1=arr1->arr[i]-borrow;
		if(diff1<0)
		{
			borrow = 1;
			diff1+=10;
		}
		else{
			borrow  = 0;
		}
		temp->arr[i]=diff1;

		i--;
	}

	int count=0;
	while(count<temp->len && temp->arr[count]==0)
		count++;


	if(count==0)
		return temp;
	
	if(count==temp->len)
	{
		temp->arr = realloc(temp->arr,1*sizeof(int));
		temp->len=1;
		temp->arr[0]=0;
		return temp;
	}
	else
	{	
		arint *temp2 = (arint *)malloc(sizeof(arint));
		temp2->arr = (int *)malloc(sizeof(int)*temp->len-count);
		temp2->len=temp->len-count;
		for(int i=count;i<temp->len;i++)
			temp2->arr[i-count]=temp->arr[i];
		
		intal_destroy(temp);
		return temp2;
	}
	return temp;



}

//Multiplies two intals and returns the product.
void* intal_multiply(void* intal1, void* intal2){
	arint *temp1 = (arint *)malloc(sizeof(arint));
	arint *temp2 = (arint *)malloc(sizeof(arint));

	arint *slave1 = (arint *)intal1;
	arint *slave2 = (arint *)intal2;

	if(slave1==NULL || slave2==NULL){
		return NULL;
	}
	temp1->arr = (int *)malloc(sizeof(int)*slave1->len);
	temp2->arr = (int *)malloc(sizeof(int)*slave2->len);

	temp1->len=slave1->len;
	temp2->len=slave2->len;	

	for(int i=0;i<slave1->len;i++)
		temp1->arr[i]=slave1->arr[i];

	for(int i=0;i<slave2->len;i++)
		temp2->arr[i]=slave2->arr[i];



	void *temp = karatsuba(temp1,temp2);
	temp = leading_zeroes(temp);
	intal_destroy(temp1);
	intal_destroy(temp2);
	return temp;
}


void* karatsuba(void* intal1, void* intal2){

	arint *temp1 = (arint *)intal1;
	arint *temp2 = (arint *)intal2;
	int flag1 = 0;
	int flag2 = 0;
	if(temp1->len<=8 && temp2->len<=8)
	{
		unsigned long long int a=0;
		unsigned long long int b=0;
		for(int i=0;i<temp1->len;i++)
			a=a*10+temp1->arr[i];
		for(int i=0;i<temp2->len;i++)
			b=b*10+temp2->arr[i];

		unsigned long long int c = a*b;
		char str[16];
		sprintf(str, "%lld",c);

		arint *temp = (arint *)malloc(sizeof(arint));
		int len1=strlen(str);
		temp->arr = (int *)malloc(sizeof(int)*len1);
		temp->len=len1;

		for(int i=0;i<temp->len;i++)	
			temp->arr[i]=str[i]-'0';

		return temp;
	}

	int lenr = temp1->len;
	if(temp1->len>temp2->len)
	{
		flag2=1;
		arint *temp3 = (arint *)intal2;

		int dlen = temp1->len-temp2->len;

		temp2 = (arint *)malloc(sizeof(arint));
		temp2->arr = (int *)malloc(sizeof(int)* temp1->len);

		
		
		temp2->len=temp1->len;
		int j=0;
		for(int i=dlen;i<temp1->len;i++)
		{
			temp2->arr[i]=temp3->arr[j++];
		} 
		for(int i=0;i<dlen;i++)
		{
			temp2->arr[i]=0;
		}

	}
	else if(temp1->len<temp2->len)
	{
		flag1=1;

		arint *temp3 = (arint *)intal1;

		int dlen = temp2->len-temp1->len;

		temp1 = (arint *)malloc(sizeof(arint));
		temp1->arr = (int *)malloc(sizeof(int)* temp2->len);

		lenr = temp2->len;

		temp1->len=temp2->len;
		int j=0;
		for(int i=dlen;i<temp2->len;i++)
		{
			temp1->arr[i]=temp3->arr[j++];
		} 
		for(int i=0;i<dlen;i++)
		{
			temp1->arr[i]=0;
		}
			
	}



	arint *onel,*oner,*twol,*twor;
	onel = (arint *)malloc(sizeof(arint));
	oner = (arint *)malloc(sizeof(arint));
	twol = (arint *)malloc(sizeof(arint));
	twor = (arint *)malloc(sizeof(arint));

	int split = lenr/2;

	onel->arr = temp1->arr;
	onel->len = split;
	oner->arr = temp1->arr+split;
	oner->len = lenr-split;

	twol->arr = temp2->arr;
	twol->len = split;
	twor->arr = temp2->arr+split;
	twor->len = lenr-split;

	//
	arint *c0,*t1,*c2,*c1;
	c2 = karatsuba(onel,twol);
	c0 = karatsuba(oner,twor);

	void *t2 = intal_add(onel,oner);
	void *t3 = intal_add(twol,twor);
	t1 = karatsuba(t2,t3);


	void *t5 = intal_add(c2,c0);
	c1 = intal_diff(t1,t5);

	if(lenr%2==1)
		lenr++;


	arint *c2t = (arint *)malloc(sizeof(arint));
	c2t->arr = (int *)malloc(sizeof(int)*(lenr+c2->len));
	c2t->len=lenr+c2->len;
	for(int i=c2->len;i<lenr+c2->len;i++)
		c2t->arr[i]=0;
	for(int i=0;i<c2->len;i++)
		c2t->arr[i]=c2->arr[i];

	lenr/=2;



	arint *c1t = (arint *)malloc(sizeof(arint));
	c1t->arr = (int *)malloc(sizeof(int)*(lenr+c1->len));
	c1t->len=lenr+c1->len;

	for(int i=c1->len;i<lenr+c1->len;i++)
		c1t->arr[i]=0;

	for(int i=0;i<c1->len;i++)
		c1t->arr[i]=c1->arr[i];


	void *x1 = intal_add(c2t,c1t);
	void *x2 = intal_add(x1,c0);


	




	if(flag1==1){
		intal_destroy(temp1);
	}
	if(flag2==1){
		intal_destroy(temp2);
	}
	intal_destroy(c2);

	intal_destroy(c0);
	intal_destroy(c1);
	intal_destroy(t1);
	intal_destroy(t2);
	intal_destroy(t3);
	intal_destroy(t5);
	intal_destroy(c1t);
	intal_destroy(c2t);
	intal_destroy(x1);

	free(onel);
	free(oner);
	free(twol);
	free(twor);
		

	return x2;




}



void* intal_divide(void* intal1, void* intal2){
	


	arint* temp1 = (arint *)intal1;
	arint* temp2 = (arint *)intal2;

	if(temp1==NULL || temp2==NULL){
		return NULL;
	}
	if(temp2->len==1 && temp2->arr[0]==0){
		return NULL;

	}
	

	if(intal_compare(intal1,intal2)==0){
		arint* temp = malloc(sizeof(arint));
		temp->arr=malloc(sizeof(int));
		temp->arr[0]=1;
		temp->len=1;
		return temp;
	}

	if(intal_compare(intal1,intal2)==-1){
		//printf("Inside if\n");
		arint* temp = malloc(sizeof(arint));
		temp->arr=malloc(sizeof(int));
		temp->arr[0]=0;
		temp->len=1;
		//printf("Value is %s\n",intal2str(temp));
		return temp;
	}


	arint *val[10];
	arint *res = temp2;
	val[0]=malloc(sizeof(arint));
	val[1]=malloc(sizeof(arint));

	val[0]->arr = malloc(sizeof(int));
	val[0]->arr[0]=0;
	val[0]->len=1;

	val[1]->arr = malloc(sizeof(int)*temp2->len);
	val[1]->len=temp2->len;
	for(int i=0;i<temp2->len;i++){
		val[1]->arr[i]=temp2->arr[i];
	}

	for(int i=2;i<10;i++){
		res = intal_add(res,temp2);
		val[i]=res;
	}

	arint *quo = malloc(sizeof(arint));
	quo->arr = malloc(sizeof(int)*(temp1->len-temp2->len+1));
	quo->len = temp1->len-temp2->len+1;

	arint *split = malloc(sizeof(arint));
	split->arr = malloc(sizeof(int)*temp2->len);
	for(int i=0;i<temp2->len;i++){
		split->arr[i]=temp1->arr[i];
	}
	arint *split1 = split;
	split->len = temp2->len;
	arint **des = malloc(sizeof(arint)*(quo->len-1));

	for(int i=0;i<quo->len;i++){

		int flag=0;
		//printf("\nValue of i is %d\nValues of split are %s\n",i,intal2str(split));
		for(int j=1;j<10;j++){
			if(intal_compare(split,val[j])==-1){
				quo->arr[i]=j-1;
				flag=1;
				break;
			}


		}

		if(flag==0)
			quo->arr[i]=9;
		
		if(i==quo->len-1)
			break;

		
		split=intal_diff(split,val[quo->arr[i]]);

		if(split->arr[0]!=0){
		split->arr = realloc(split->arr,sizeof(int)*(split->len+1));
		split->arr[split->len]=temp1->arr[temp2->len+i];
		split->len++;
		}
		else
			split->arr[0]=temp1->arr[temp2->len+i];
		

		des[i] = split;


	}
	
	//intal_destroy(val[0]);
	for(int i=0;i<10;i++){
		//printf("Before is %s %d\n",intal2str(val[i]),val[i]->len);
		intal_destroy(val[i]);
		//printf("After is %s\n",intal2str(val[i]),val[i]->len);
	}
	//intal_destroy(split);
	

	for(int i=0;i<quo->len-1;i++){
		intal_destroy(des[i]);
	}
	free(des);
	intal_destroy(split1);
	//printf("Quotient is %s\n",intal2str(quo));
	quo = leading_zeroes(quo);
	return quo;


}



//Returns -1, 0, +1
//Returns 0 when both are equal.
//Returns +1 when intal1 is greater, and -1 when intal2 is greater.
int intal_compare(void* intal1, void* intal2){
	arint *temp1 = (arint *)intal1;
	arint *temp2 = (arint *)intal2;

	if(temp1==NULL || temp2==NULL){
		return -2;
	}
	if(temp1->len>temp2->len)
		return 1;
	else if(temp1->len<temp2->len)
		return -1;
	for(int i=0;i<temp1->len;i++){
		if(temp1->arr[i]>temp2->arr[i])
			return 1;
		else if(temp1->arr[i]<temp2->arr[i])	
			return -1;

	}
	return 0;
}








void* intal_pow(void* intal1, void* intal2){

	arint *temp1 = (arint *)intal1;
	arint *temp2 = (arint *)intal2;

	if(temp1==NULL || temp2==NULL){
		return NULL;
	}
	if(temp1->len==1 && temp1->arr[0]==0){
		arint *rer = malloc(sizeof(arint));
		rer->arr = malloc(sizeof(int));
		rer->len=1;
		rer->arr[0]=0;
		return rer;
	}

	if(temp2->len==1 && temp2->arr[0]==0){
		arint *rer = malloc(sizeof(arint));
		rer->arr = malloc(sizeof(int));
		rer->len=1;
		rer->arr[0]=1;
		return rer;
	}


	if(temp1->len==1 && temp1->arr[0]==1){
		arint *rer = malloc(sizeof(arint));
		rer->arr = malloc(sizeof(int));
		rer->len=1;
		rer->arr[0]=1;
		return rer;
	}

	if(temp2->len==1 && temp2->arr[0]==1){
		arint *rer = malloc(sizeof(arint));
		rer->arr = malloc(sizeof(int)*temp1->len);
		rer->len=temp1->len;
		for(int i=0;i<temp1->len;i++){
			rer->arr[i]=temp1->arr[i];
		}
		return rer;
	}

	arint *res = power_recursive(intal1,intal2);
	res=leading_zeroes(res);

	return res;

}
void* power_recursive(void* intal1,void* intal2){

	arint *temp1 = (arint *)intal1;
	arint *temp2 = (arint *)intal2;
	arint *div = malloc(sizeof(arint));
	div->arr = malloc(sizeof(int));
	div->arr[0]=2;
	div->len = 1;

	if(temp2->len==1 && temp2->arr[0]==1)
		return temp1;

	if(temp2->arr[temp2->len-1]%2==0){
		arint *temp3 = intal_divide(intal2,div);
		
		arint *temp4 = power_recursive(intal1,temp3);

		arint *temp5 = intal_multiply(temp4,temp4);

		return temp5;
	}
	else{
		arint *temp4 = intal_decrement(intal2);

		arint *temp5 = power_recursive(intal1,temp4);

		arint *temp6 = intal_multiply(intal1,temp5);

		return temp6;
	}
}

void* leading_zeroes(void* intal1){
	
	arint* temp=(arint *)intal1;
	int i=0;

	while(i<temp->len && temp->arr[i]==0)
		i++;
	

	if(i==0)
		return intal1;
	if(i==temp->len){
		arint* rer = malloc(sizeof(arint));
		rer->arr = malloc(sizeof(int));
		rer->len=1;
		rer->arr[0]=0;
		intal_destroy(temp);
		return rer;

	}
	arint *rer = malloc(sizeof(arint));
	rer->arr = malloc(sizeof(int)*(temp->len-i));
	rer->len = (temp->len-i);
	
	for(int k=i;k<temp->len;k++)
		rer->arr[k-i]=temp->arr[k];
	intal_destroy(temp);
	return rer;


}