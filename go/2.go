package main

import (
	"fmt"
	"math/rand"
	"strings"
	"time"
)

func GENERATE_MASTER_KEY(MASTER_KEY *[]byte) {
	letters := 'a' + rand.New(rand.NewSource(time.Now().UnixNano())).Int31n(26)
	numbers := '0' + rand.New(rand.NewSource(time.Now().UnixNano())).Int31n(10)
	for i := 0; i < 8; i++ {
		*MASTER_KEY = append(*MASTER_KEY, byte(letters))
		*MASTER_KEY = append(*MASTER_KEY, byte(numbers))
	}
}

func ShiftRows(line []byte) {
	v := make([]byte, len(line))
	for i := 1; i < len(line); i++ {
		v[i-1] = line[i]
	}
	v[len(line)-1] = line[0]
	copy(line, v)
}

func SubBytes(line []byte) {
	Sbox := []byte{
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
       0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
       0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
       0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
       0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
       0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
       0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
       0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
       0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
       0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
       0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
       0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
       0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
       0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
       0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
       0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	}
	v := make([]byte, len(line))
	for i, b := range line {
		v[i] = Sbox[b]
	}
	copy(line, v)
}

func AddRoundKey(FIRST, SECOND []byte) []byte {
	if len(FIRST) != len(SECOND) {
		panic("Input slices must have the same length")
	}

	result := make([]byte, len(FIRST))
	for i := range FIRST {
		result[i] = FIRST[i] ^ SECOND[i]
	}
	return result
}
func KeyExpansion(MASTER_KEY []byte, ROUND_KEYS *[][][]byte) {
	Rcon := [][]byte{
		{0x00, 0x00, 0x00, 0x00},
		{0x01, 0x00, 0x00, 0x00},
		{0x02, 0x00, 0x00, 0x00},
		{0x04, 0x00, 0x00, 0x00},
		{0x08, 0x00, 0x00, 0x00},
		{0x10, 0x00, 0x00, 0x00},
		{0x20, 0x00, 0x00, 0x00},
		{0x40, 0x00, 0x00, 0x00},
		{0x80, 0x00, 0x00, 0x00},
		{0x1b, 0x00, 0x00, 0x00},
		{0x36, 0x00, 0x00, 0x00},
	}

	Nk := 4
	Nb := 4
	Nr := 10

	*ROUND_KEYS = make([][][]byte, Nb*(Nr+1))
	for i := range *ROUND_KEYS {
		(*ROUND_KEYS)[i] = make([][]byte, 4)
		for j := range (*ROUND_KEYS)[i] {
			(*ROUND_KEYS)[i][j] = make([]byte, 4)
		}
	}

	v := make([]byte, 4)
	i := 0
	for i < Nk {
		copy(v, MASTER_KEY[4*i:4*i+4])
		(*ROUND_KEYS)[i] = v
		i++
	}

	i = Nk
	for i < Nb*(Nr+1) {
		v = (*ROUND_KEYS)[i-1]
		if i%Nk == 4 {
			ShiftRows(v)
			SubBytes(v)
			for k := range v {
				v[k] ^= Rcon[i/Nk][k]
			}
		} else if Nk > 6 && i%Nk == 4 {
			SubBytes(v)
		}

		for j := range v {
			(*ROUND_KEYS)[i][j] = (*ROUND_KEYS)[i-Nk][j] ^ v[j]
		}
		i++
	}
}

func galoisMultiply(a, b byte) byte {
	result := byte(0)
	for i := 0; i < 8; i++ {
		if b&1 == 1 {
			result ^= a
		}
		carry := a & 0x80
		a <<= 1
		if carry != 0 {
			a ^= 0x1b
		}
		b >>= 1
	}
	return result
}

func generateMultBy2() []byte {
	multBy2 := make([]byte, 256)
	for i := range multBy2 {
		multBy2[i] = galoisMultiply(byte(i), 2)
	}
	return multBy2
}

func generateMultBy3() []byte {
	multBy3 := make([]byte, 256)
	for i := range multBy3 {
		multBy3[i] = galoisMultiply(byte(i), 3)
	}
	return multBy3
}

func mixColumn(line []byte) {
	multBy2 := generateMultBy2()
	multBy3 := generateMultBy3()

	v := make([]byte, 4)
	v[0] = multBy2[line[0]] ^ multBy3[line[1]] ^ line[2] ^ line[3]
	v[1] = multBy2[line[1]] ^ multBy3[line[2]] ^ line[0] ^ line[3]
	v[2] = multBy2[line[2]] ^ multBy3[line[3]] ^ line[0] ^ line[1]
	v[3] = multBy2[line[3]] ^ multBy3[line[0]] ^ line[1] ^ line[2]
	copy(line, v)
}

func XORBlocks(a, b [][]byte) [][]byte {
	res := make([][]byte, 4)
	for i := range res {
		res[i] = make([]byte, 4)
		for j := range res[i] {
			res[i][j] = a[i][j] ^ b[i][j]
		}
	}
	return res
}

func BLOCK_GENERATE(text string, BLOCK *[][][]byte) {
	for len(text)%16 != 0 {
		text += " "
	}

	*BLOCK = make([][][]byte, len(text)/16)
	for i := range *BLOCK {
		(*BLOCK)[i] = make([][]byte, 4)
		for j := range (*BLOCK)[i] {
			(*BLOCK)[i][j] = make([]byte, 4)
		}
	}

	for i, r := range text {
		a := i % 16
		b := a / 4
		(*BLOCK)[i/16][a%4][b] = byte(r)
	}
}

func CIPHER(BLOCK [][]byte, ROUND_KEYS [][]byte, MASTER_KEY []byte) [][]byte {
	v := make([][]byte, 4)
	for i := range v {
		v[i] = make([]byte, 4)
	}

	for i := range BLOCK {
		v[i] = AddRoundKey(BLOCK[i], ROUND_KEYS[i])
	}

	for i := 1; i <= 9; i++ {
		for j := range BLOCK {
			SubBytes(v[j])
			ShiftRows(v[j])
			mixColumn(v[j])
			v[j] = AddRoundKey(v[j], ROUND_KEYS[i])
		}
	}

	for j := range BLOCK {
		SubBytes(v[j])
		ShiftRows(v[j])
		v[j] = AddRoundKey(v[j], ROUND_KEYS[10])
	}

	return v
}
func main() {
	rand.Seed(time.Now().UnixNano())
	var MASTER_KEY []byte
	var ROUND_KEYS [][][]byte
	var BLOCK [][][]byte

	var text string
	fmt.Println("enter text for encryption ==>> ")
	fmt.Scanln(&text)

	BLOCK_GENERATE(text, &BLOCK)
	fmt.Println("----------------------------------------------")
	fmt.Println("\nencryption block looks like this: \n")
	for _, block := range BLOCK {
		for _, row := range block {
			for _, b := range row {
				fmt.Printf("%4d ", b)
			}
			fmt.Println()
		}
		fmt.Println("\n------")
	}
	fmt.Println("\n----------------------------------------------\n")

	GENERATE_MASTER_KEY(&MASTER_KEY)
	fmt.Println("\nmaster key 128 bit: ")
	for _, b := range MASTER_KEY {
		fmt.Printf("%c", b)
	}
	fmt.Println("\n\n----------------------------------------------\n")

	KeyExpansion(MASTER_KEY, &ROUND_KEYS)
	fmt.Println("generated keys \n----------------------------------------------\n")
	for _, roundKey := range ROUND_KEYS {
		for _, b := range roundKey {
			fmt.Printf("%04x ", b)
		}
		fmt.Println()
	}
	fmt.Println("\n----------------------------------------------\n")

	var PREV, DEFOLT [][]byte
	DEFOLT = make([][]byte, 4)
	for i := range DEFOLT {
		DEFOLT[i] = make([]byte, 4)
	}

	var ECRYPT [][][]byte
	for i := range BLOCK {
		res := make([][]byte, 4)
		for j := range res {
			res[j] = make([]byte, 4)
		}
		B := BLOCK[i]
		TEK := CIPHER(PREV, ROUND_KEYS, MASTER_KEY)
		for i := range TEK {
			for j := range TEK[i] {
				res[i][j] = TEK[i][j] ^ B[i][j]
			}
		}
		ECRYPT = append(ECRYPT, res)
		PREV = TEK
	}

	fmt.Println("final cipher after encryption:\n")
	for _, cipherBlock := range ECRYPT {
		for _, row := range cipherBlock {
			for _, b := range row {
				fmt.Printf("%4d ", b)
			}
			fmt.Println()
		}
	}
	fmt.Println("\n\n----------------------------------------------\n")

	PREV = DEFOLT
	var DECRYPT [][][]byte
	for i := range ECRYPT {
		res := make([][]byte, 4)
		for j := range res {
			res[j] = make([]byte, 4)
		}
		B := ECRYPT[i]
		TEK := CIPHER(PREV, ROUND_KEYS, MASTER_KEY)
		for i := range TEK {
			for j := range TEK[i] {
				res[i][j] = B[i][j] ^ TEK[i][j]
			}
		}
		DECRYPT = append(DECRYPT, res)
		PREV = TEK
	}

	fmt.Println("received message after decryption:\n")
	for _, decrypt := range DECRYPT {
		for _, row := range decrypt {
			for _, b := range row {
				fmt.Printf("%c", b)
			}
		}
	}
	fmt.Println("\n\n----------------------------------------------\n")
}