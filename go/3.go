package main

import (
	"fmt"
	"math"
)

func LU(LEFT [][]float64, RIGHT []float64) {
	L := make([][]float64, 4)
	U := make([][]float64, 4)
	for i := range L {
		L[i] = make([]float64, 4)
		U[i] = make([]float64, 4)
	}

	for i := 0; i < 4; i++ {
		for j := i; j < 4; j++ {
			U[i][j] = LEFT[i][j]
			for k := 0; k < i; k++ {
				U[i][j] -= L[i][k] * U[k][j]
			}
		}

		for j := i + 1; j < 4; j++ {
			L[j][i] = LEFT[j][i]
			for k := 0; k < i; k++ {
				L[j][i] -= L[j][k] * U[k][i]
			}
			L[j][i] /= U[i][i]
		}
	}

	fmt.Println("\nL - матрица ====================================")
	for _, row := range L {
		for _, val := range row {
			fmt.Printf("%f\t", val)
		}
		fmt.Println()
	}

	fmt.Println("\nU - матрица ====================================")
	for _, row := range U {
		for _, val := range row {
			fmt.Printf("%f\t", val)
		}
		fmt.Println()
	}

	y := make([]float64, 4)
	for i := 0; i < 4; i++ {
		y[i] = RIGHT[i]
		for j := 0; j < i; j++ {
			y[i] -= L[i][j] * y[j]
		}
	}

	x := make([]float64, 4)
	for i := 3; i >= 0; i-- {
		x[i] = y[i]
		for j := i + 1; j < 4; j++ {
			x[i] -= U[i][j] * x[j]
		}
		x[i] /= U[i][i]
	}

	fmt.Println()
	for i, val := range x {
		fmt.Printf("x%d = %f\n", i+1, val)
	}
}

func gaussSeidel(A [][]float64, b []float64, x []float64) {
	const maxIterations = 99
	const epsilon = 0.0001
	n := len(A)

	for k := 0; k < maxIterations; k++ {
		xOld := make([]float64, n)
		copy(xOld, x)

		for i := 0; i < n; i++ {
			sum := 0.0
			for j := 0; j < n; j++ {
				if j != i {
					sum += A[i][j] * x[j]
				}
			}
			x[i] = (b[i] - sum) / A[i][i]
		}

		fmt.Printf("Iteration %d: ", k+1)
		for _, xi := range x {
			fmt.Printf("%f ", xi)
		}
		fmt.Println()

		convergence := false
		for i := 0; i < n; i++ {
			if math.Abs(x[i]-xOld[i]) < epsilon {
				convergence = true
				break
			}
		}

		if convergence {
			fmt.Println("Сошлось!")
			return
		}
	}
	fmt.Println("Не сошлось!")
}

func main() {
	LEFT := [][]float64{
		{1.08, -0.04, 0.21, -18},
		{0.25, -1.23, 0.22, -0.09},
		{-0.21, 0.22, 0.8, -0.13},
		{0.15, -1.31, 0.06, -1.16},
	}
	RIGHT := []float64{-1.24, -1.16, 2.56, 1.08}

	f := make([]float64, 4)
	for i := 0; i < 4; i++ {
		f[i] = RIGHT[i] / LEFT[i][0]
	}

	C := make([][]float64, 4)
	for i := range C {
		C[i] = make([]float64, 4)
	}

	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			if i != j {
				C[i][j] = -(LEFT[i][j] / LEFT[i][i])
			}
		}
	}
	fmt.Println("МАТРИЦА C-------------------------")
	for _, row := range C {
		for _, val := range row {
			fmt.Printf("%.3f\t\t", val)
		}
		fmt.Println()
	}
	fmt.Println("\nМАТРИЦА F-------------------------")
	for _, val := range f {
		fmt.Println(val)
	}

	x_k := []float64{0, 0, 0, 0}
	gaussSeidel(LEFT, RIGHT, x_k)
	LU(LEFT, RIGHT)
}
