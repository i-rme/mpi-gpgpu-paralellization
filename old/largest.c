int largest(int arr[], int n){
  int i;
  int max = arr[0]; //Initialize maximum element
  for (i=1; i<n; i++) if(arr[i]>max) max=arr[i];
  return max;
}

int main()
{
	int arr[]={100, 200, 31, 13, 97, 10, 20, 11};
    int n = sizeof(arr)/sizeof(arr[0]);
    printf("Largest number in given array is %d", largest(arr, n));
    return 0;
}
