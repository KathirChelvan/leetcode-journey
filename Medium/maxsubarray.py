class Solution:
    def maxSubArray(self, nums):
        sum, maxSum = 0, float('-inf')
        for i in range(len(nums)):
            sum += nums[i]
            if sum > maxSum:
                maxSum = sum
            if sum < 0:
                sum = 0
        return maxSum