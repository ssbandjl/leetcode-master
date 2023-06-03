package main

import (
	"fmt"
)

func isHappy(n int) bool {
	m := map[int]bool{}
	for ; n != 1 && !m[n]; n, m[n] = step(n), true {
	}
	return n == 1
}

func step(n int) int {
	sum := 0
	for n > 0 {
		sum += (n % 10) * (n % 10)
		n = n / 10
	}
	return sum
}

func main() {
	fmt.Println(isHappy(19))
}

// 作者：LeetCode-Solution
// 链接：https://leetcode.cn/problems/happy-number/solution/kuai-le-shu-by-leetcode-solution/
