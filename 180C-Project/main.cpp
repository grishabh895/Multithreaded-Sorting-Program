#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>
using namespace std::chrono;
using namespace std;
void *runner_merge_sort(void *param); /* threads call this function */

class data_base {
public:
    int *list;
    int length;
    int thread_number;
    data_base(int l) {
        length = l;
        list = (int*)malloc(length*sizeof(int));
    }
    void set_list(int num, int index);
    int get_list(int index);
    void merge_sort_multithreading(data_base data);
    void quick_sort_multithreading(data_base data);
};


void data_base::set_list(int num, int index) {
    list[index] = num;
}
int data_base::get_list(int index) {
   return list[index];
}

void merge(int arr[], int left, int middle, int right) {

    int i, j, k;
    int half1 = middle - left + 1;
    int half2 = right - middle;

    int first[half1], second[half2]; // temp arrays

    for (i = 0; i < half1; i++) {
        first[i] = arr[left + i];
    }

    for (j = 0; j < half2; j++) {
        second[j] = arr[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < half1 && j < half2) {

        if (first[i] <= second[j]) {
            arr[k] = first[i];
            ++i;
        } else {
            arr[k] = second[j];
            j++;
        }

        k++;
    }

    while (i < half1) {
        arr[k] = first[i];
        i++;
        k++;
    }

    while (j < half2) {
        arr[k] = second[j];
        j++;
        k++;
    }
}

// A utility function to swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(int arr[], int low, int high)
{
    int arrsize = high - low + 1;
    if ((low < high) && arrsize <= 20)
    {
        // insertion sort algorithm
        for (unsigned int i = low; i <= high; i++) {
            int temp = arr[i];
            unsigned int j = i;
            while (j > low && temp < arr[j - 1]) {
                arr[j] = arr[j - 1];
                j--;
            }
            arr[j] = temp;
        }
    } else {
        if (low < high)
        {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
}

void *runner_quick_sort(void *param)
{
    data_base *data = (data_base*)param;
    int midpoint = data->length/2;
    static int thread = 0;
    thread++;
    if (thread == 1) {
        cout << "Thread 1: 0-" << midpoint <<endl;
        quickSort(data->list, 0, midpoint);
        pthread_exit(0);
    }

   if (thread == 2) {
        cout << "Thread 2: "<< midpoint + 1 << "-" << data->length - 1 <<endl;
        quickSort(data->list, midpoint + 1, data->length - 1);
        pthread_exit(0);
    }

    if (thread == 3) {
        merge(data->list, 0, (data->length/2), data->length-1);
        pthread_exit(0);
    }
}

void data_base::quick_sort_multithreading(data_base data) {
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */

    pthread_t tid2; // second thread id
    pthread_attr_t attr2; // second thread attributes

    pthread_t tid3; // third thread id
    pthread_attr_t attr3;

    pthread_attr_init(&attr); /* get the default attributes */
    pthread_create(&tid,&attr,runner_quick_sort, &data); /* create the thread */

    pthread_attr_init(&attr2);
    pthread_create(&tid2,&attr2, runner_quick_sort, &data);
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);

    pthread_attr_init(&attr3);
    pthread_create(&tid3, &attr2, runner_quick_sort, &data);
    pthread_join(tid3, NULL);

    int j;
    for (j = 0; j < data.length; j++) {
        if (j == data.length-1) {
            printf("%d\n", data.list[j]);
        } else {
            printf("%d\t", data.list[j]);
        }
    }

}

void merge_sort(int arr[], int l, int r) {
        int arrsize = r - l + 1;
    if ((l < r) && arrsize <= 20)
    {
        // insertion sort algorithm
        for (unsigned int i = l; i <= r; i++) {
            int temp = arr[i];
            unsigned int j = i;
            while (j > l && temp < arr[j - 1]) {
                arr[j] = arr[j - 1];
                j--;
            }
            arr[j] = temp;
        }
    } else {
        if (l < r) {
            int middle = (l + (r-1))/ 2;
            merge_sort(arr, l, middle);
            merge_sort(arr, middle+1, r);
            merge(arr, l, middle, r);
        }
    }
}

void *runner_merge_sort(void *param)
{
    data_base *data = (data_base*)param;
    int midpoint = data->length/2;
    static int thread = 0;
    thread++;
    if (thread == 1) {
        cout << "Thread 1: 0-" << midpoint <<endl;
        merge_sort(data->list, 0, midpoint);
        pthread_exit(0);
    }

   if (thread == 2) {
        cout << "Thread 2: "<< midpoint + 1 << "-" << data->length - 1 <<endl;
        merge_sort(data->list, midpoint + 1, data->length - 1);
        pthread_exit(0);
    }

    if (thread == 3) {
        merge(data->list, 0, (data->length/2), data->length-1);
        pthread_exit(0);
    }


}

void data_base::merge_sort_multithreading(data_base data) {
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */

    pthread_t tid2; // second thread id
    pthread_attr_t attr2; // second thread attributes

    pthread_t tid3; // third thread id
    pthread_attr_t attr3;

    clock_t t1, t2;
    t1 = clock();
    pthread_attr_init(&attr); /* get the default attributes */
    pthread_create(&tid,&attr,runner_merge_sort, &data); /* create the thread */

    pthread_attr_init(&attr2);
    pthread_create(&tid2,&attr2, runner_merge_sort, &data);
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);

    pthread_attr_init(&attr3);
    pthread_create(&tid3, &attr2, runner_merge_sort, &data);
    pthread_join(tid3, NULL);
    t2 = clock();

    int j;
    for (j = 0; j < data.length; j++) {
        if (j == data.length-1) {
            printf("%d\n", data.list[j]);
        } else {
            printf("%d\t", data.list[j]);
        }
    }

}


int main()
{
    int length, sorting_option;
    printf("Enter the number of elements \n");
    scanf("%d", &length);
    data_base data1(length);
    printf("Below are the random numbers generated \n");
    srand (time(NULL));
    for (int i =0; i<length ; i++) {
        data1.set_list(rand()%(10*length), i);
        printf("%d\t", data1.get_list(i));
    }
    printf("\nPress 1 for QuickSort\nPress 2 for MergeSort\n");

    scanf("%d", &sorting_option);
    auto start = high_resolution_clock::now();
    switch(sorting_option) {
    case 1:
        cout << "Quick Sort ======================>" <<endl;
        data1.quick_sort_multithreading(data1);
        break;
    case 2:
        cout << "Merge Sort ======================>" <<endl;
        data1.merge_sort_multithreading(data1);
        break;
    default:
        break;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken "<< duration.count() << " microseconds" <<endl;
    return 0;
}

