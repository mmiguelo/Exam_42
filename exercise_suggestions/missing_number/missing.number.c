int missing_number(int* nums)
{
    int actual_sum = 0;
    int expected_sum;
    int n = 0;
    int i = 0;

    while (1)
    {
        if (nums[i] > n)
            n = nums[i];  
        actual_sum += nums[i];  
        i++;
        if (nums[i] < 0 || nums[i] > n + 1) 
            break;
    }
    expected_sum = (n *(n + 1)) / 2;
    return (expected_sum - actual_sum);
}