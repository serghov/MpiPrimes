#include <mpi.h>
#include <stdio.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;
/*
   checks wheather or not a number is prime with in O(n) time
 */
bool isPrime(int num)
{	
	if (num%2==0)
		return 0;
	for (int i=3;i<sqrt(num);i+=2)
		if (num%i==0)
			return 0;
	return 1;
}

int n;
int world_size; //number of processes involved in our calculations

int *curPrimes;//the primes that we will calculate in THIS process
int k;

int *recvPrimes;//temperary variable for primes the main process will recieve from secondary ones

vector <int> allPrimes;

int main(int argc, char** argv) {

	int i;
	//check if we have the right number of arguments
	if (argc!=2)
	{
		cout <<"invalid number of arguments"<<endl;
		return 0;
	}
	//calculate n as per requrenment
	n = 100000 * atoi(argv[1]);
	//we cant have more than n/2 primes becuase even numbers for sure cant be prime
	curPrimes = new int[n/2];

	//init mpi
	MPI_Init(NULL, NULL);

	//find out how many processes are there
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	//find out which process are we in
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	//calculate prime numbers each process will calculate its own portion
	for (i = n*world_rank / world_size;i < n*(world_rank + 1) / world_size; i++)
		if (isPrime(i))
			curPrimes[k++]=i;
	//if we are not in the main process(process with rank 0) send our prime numbers to main process
	if (world_rank!=0)
	{
		MPI_Send(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(curPrimes, k, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}


	int recvSize;



	if (world_rank==0)
	{
		//this runs only on main process
		//add the primes calculated here to our large prime vector
		for (i=0;i<k;i++)    	
			allPrimes.push_back(curPrimes[i]);
		//clear the memory of our primes
		delete curPrimes;
		//go through all the recieved primes and add them to our vector
		for (i=1;i<world_size;i++)
		{
			MPI_Recv(&recvSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout <<"Recieved "<<recvSize<<" primes\nretrieving"<<endl;
			recvPrimes = new int[recvSize];
			MPI_Recv(recvPrimes, recvSize, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j=0;j<recvSize;j++)    	
				allPrimes.push_back(recvPrimes[j]);
			delete recvPrimes;
		}
		
		cout <<"overal "<<allPrimes.size()<<" primes"<<endl;
		//output all primes to a file
		freopen("primes.txt","w",stdout);

		for(i=0;i<allPrimes.size();i++)
			printf("%d ", allPrimes[i]);
		printf("\n");
	}



	MPI_Finalize();
	return 0;
}
