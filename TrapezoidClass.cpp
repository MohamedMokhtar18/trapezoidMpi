#include<stdio.h>
#include<mpi.h>
int main(int argc,char** argv) {
	int my_rank;//processor rank
	int p;//number of processors
	float a=0.0, b=1.0;//left and right point
	int n=1024;//number of trapezoid
	float h;//trapezoid per width
	float x;
	float local_a;//left endpoint my process
	float local_b;//right endpoint my process
	int local_n;//number of trapezoids for my calculation
	float integral;//store result
	float total;//total integral
	int source;//process sending integral
	int dest = 0;//all messages goto 0
	int tag = 0;
	MPI_Status status;
	float Trap(float local_a, float local_b, int local_n, float h);//calculate local integral
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);//get process rank
	MPI_Comm_size(MPI_COMM_WORLD, &p);//see how many processor used

	//float f(float x); //function we are integrating
	//printf("Enter a,b and n \n");
	//scanf("%f %f %d",&a,&b,&n);
	h = (b - a) / n;//the same number for all process
	local_n = n / p;//the number of trapezoids
	/*lenth of each process interval 
	of integration=local_n*h so the interval starts at:
	*/
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	integral = Trap(local_a, local_b, local_n, h);
	//integral = (f(a) + f(b)) / 2.0;
	if (my_rank==0)
	{
		total = integral;
		for (source = 1; source < p ; source++)
		{
			MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
			total = total + integral;
		/*	x = x + h;
			integral = integral + f(x);*/
		}
	}
	else {
		MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
	}

	//x = a;
	if (my_rank==0)
	{
		printf("With n = %d trapezoids.our estimate \n", n	);
		printf("of the integral from %f to %f = %f\n", a, b, total); 
	}
	MPI_Finalize();
	/*integral = integral * h;
	printf("With n = %d trapezoids.our estimate \n",n);
	printf("of the integral from %f to %f = %f\n",a,b,integral);*/
}
float Trap(float local_a, float local_b, int local_n, float h) {
	float integral;
	float x;
	int i;
	float f(float x);
	integral = (f(local_a) + f(local_b)) / 2.0;
	x = local_a;
	for (int i = 0; i < local_n-1; i++)
	{
		x = x + h;
		integral = integral + f(x);
	}
	integral = integral * h;
	return integral;
}
float f(float x) {
	float return_val;
	return_val = x + 2;
	return return_val;
}