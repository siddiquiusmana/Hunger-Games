Project: Hunger Games

This program utilizes Linked Lists to thin out a group of prisoners through two separate execution 
phases until only one survivor remains.

Program reads an in.txt as input and stores it in dynamically allocated memory blocks.

The in.txt file is formatted as follows:

----------------------------------------------------
#ofPities

Pity1 PityName #ofPrisoners k th
Pity2 PityName #ofPrisoners k th
[...]
Pity# PityName #ofPrisoners k th

----------------------------------------------------

Where Pity#, #ofPrisoners, k, and th are all positive integers only

The program creates individual prisoners in each pity and labels them accordingly. Then, it 
reverses their order so they stand in ascending order. Following two phase execution operations, 
prisoners are executed until only one prisoner remains. 

During Phase one, every kth prisoner is killed until th number of prisoners are remaining in the
pity. During Phase two, prisoners with largest prisoner numbers and largest pity numbers are 
executed until one remains. Each of the executions are printed out into an out.txt file.