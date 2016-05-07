#include <mpi.h>
#include <stdio.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

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
int world_size;

int *curPrimes;
int k;

int *recvPrimes;

vector <int> allPrimes;

int main(int argc, char** argv) {

	int i;

	if (argc!=2)
	{
		cout <<"invalid number of arguments"<<endl;
		return 0;
	}

	n = 100000 * atoi(argv[1]);

	curPrimes = new int[n/2];


    MPI_Init(NULL, NULL);
 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    


    for (i = n*world_rank / world_size;i < n*(world_rank + 1) / world_size; i++)
    	if (isPrime(i))
    		curPrimes[k++]=i;

    if (world_rank!=0)
    {
    	MPI_Send(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    	MPI_Send(curPrimes, k, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    //

    int recvSize;



    if (world_rank==0)
    {
    	for (i=0;i<k;i++)    	
    		allPrimes.push_back(curPrimes[i]);
    	
    	delete curPrimes;

    	for (i=1;i<world_size;i++)
    	{
    		MPI_Recv(&recvSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		cout <<"Recieved "<<recvSize<<" primes\nretrieving"<<endl;
    		recvPrimes = new int[recvSize];
    		MPI_Recv(recvPrimes, recvSize, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j=0;j<recvSize;j++)    	
    			allPrimes.push_back(recvPrimes[i]);
    		delete recvPrimes;
    	}

    	cout <<"overal "<<allPrimes.size()<<" primes"<<endl;
    	//for(i=0;i<allPrimes.size();i++)
	    //	printf("%d ", allPrimes[i]);
    	//cout <<endl;
    }

    

    MPI_Finalize();
    return 0;
}