package main

import (
	"fmt"
)

func isHappy(n int) bool {
	cycle := map[int]bool{4: true, 6: true, 37: true, 58: true, 89: true, 145: true, 42: true, 20: true}
	for n != 1 && !cycle[n] {
		n = step(n)
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
