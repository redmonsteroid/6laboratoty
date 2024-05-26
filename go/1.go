package main

import (
	"fmt"
	"math/rand"
	"time"
)

func isPrime(num int) bool {
	if num < 2 {
		return false
	}
	if num == 2 {
		return true
	}
	if num%2 == 0 {
		return false
	}
	for i := 3; i*i <= num; i += 2 {
		if num%i == 0 {
			return false
		}
	}
	return true
}

func task1() {
	rand.Seed(time.Now().UnixNano())
	fmt.Println("Enter size of matrix")
	var m, n int
	fmt.Scan(&m, &n)
	matrix := make([][]int, m)
	for i := range matrix {
		matrix[i] = make([]int, n)
	}

	for i := 0; i < m; i++ {
		for j := 0; j < n; j++ {
			matrix[i][j] = rand.Intn(51)
		}
	}

	fmt.Println("Matrix is")
	for i := 0; i < m; i++ {
		for j := 0; j < n; j++ {
			fmt.Print(matrix[i][j], " ")
		}
		fmt.Println()
	}

	primeCount := make(map[int]int)
	for _, row := range matrix {
		for _, num := range row {
			if isPrime(num) {
				primeCount[num]++
			}
		}
	}

	var maxPrime, maxCount int
	for prime, count := range primeCount {
		if count > maxCount {
			maxPrime = prime
			maxCount = count
		}
	}

	if maxCount > 0 {
		fmt.Printf("The prime number that occurs the most times: %d (occurs %d times)\n", maxPrime, maxCount)
	} else {
		fmt.Println("There are no prime numbers in the matrix.")
	}
}

func task2() {
	fmt.Println("Enter size of matrix: ")
	var N int
	fmt.Scan(&N)
	M := N

	matrix := make([][]int, N)
	for i := range matrix {
		matrix[i] = make([]int, N)
	}

	for i := 0; i < M; i++ {
		for j := 0; j < N; j++ {
			if i == j {
				matrix[i][j] = 0
			} else if i > j {
				matrix[i][j] = 100 + j
			} else {
				matrix[i][j] = 100 + N - j - 1
			}
		}
	}

	for i := 0; i < M; i++ {
		for j := 0; j < N; j++ {
			if matrix[i][j] == 0 {
				fmt.Print("000", " ")
			} else {
				fmt.Print(matrix[i][j], " ")
			}
		}
		fmt.Println()
	}
}

func generateUniqueRandomNumbers(count, min, max int) []int {
	uniqueNumbers := make(map[int]bool)
	numbers := make([]int, 0, count)

	for len(uniqueNumbers) < count {
		num := min + rand.Intn(max-min+1)
		if !uniqueNumbers[num] {
			uniqueNumbers[num] = true
			numbers = append(numbers, num)
		}
	}

	return numbers
}

func findSpecialElements(matrix [][]int) []int {
	specialElements := []int{}
	rows := len(matrix)
	cols := len(matrix[0])

	for i := 0; i < rows; i++ {
		minElement := matrix[i][0]
		minIndex := 0
		for j := 1; j < cols; j++ {
			if matrix[i][j] < minElement {
				minElement = matrix[i][j]
				minIndex = j
			}
		}

		isMaxInColumn := true
		for k := 0; k < rows; k++ {
			if matrix[k][minIndex] > minElement {
				isMaxInColumn = false
				break
			}
		}

		if isMaxInColumn {
			specialElements = append(specialElements, minElement)
		}
	}

	return specialElements
}

func task3() {
	rand.Seed(time.Now().UnixNano())

	M := 5
	N := 5

	randomNumbers := generateUniqueRandomNumbers(M*N, 1, 100)
	rand.Shuffle(len(randomNumbers), func(i, j int) {
		randomNumbers[i], randomNumbers[j] = randomNumbers[j], randomNumbers[i]
	})

	matrix := make([][]int, M)
	for i := range matrix {
		matrix[i] = make([]int, N)
	}

	index := 0
	for i := 0; i < M; i++ {
		for j := 0; j < N; j++ {
			matrix[i][j] = randomNumbers[index]
			index++
		}
	}

	fmt.Println("Matrix:")
	for _, row := range matrix {
		for _, val := range row {
			fmt.Print(val, " ")
		}
		fmt.Println()
	}

	specialElements := findSpecialElements(matrix)
	fmt.Println("Special elements (minimal in row, maximal in column):")
	for _, val := range specialElements {
		fmt.Print(val, " ")
	}
	fmt.Println()
}

func main() {
	fmt.Println("Enter task number")
	var task int
	fmt.Scan(&task)
	fmt.Println()
	switch task {
	case 1:
		task1()
	case 2:
		task2()
	case 3:
		task3()
	default:
		fmt.Println("Incorrect task number")
	}
}
