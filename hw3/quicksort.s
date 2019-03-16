# CS 110 Spring 2019 hw3
# quick_sort.s

#========================================================================================
#                              Quick Sort RISC-V
#                                   README
#========================================================================================
# In this file you will be implementing the def_quick_sort and def_partition 
# function, which form the most important part of this code.
#
# You should sort a 10-integer array. These arrays was reserved in static
# partition of memory. You can change to any 10 numbers you want to sort for
# testing and we will change them to our test case for grading.
# 
# We give you the choice to test your code using predefined array: in line 41.
# You can decomment it and use array to test your code.
# 
# 
#                               IMPORTANT!!!
# Our asembler will support the following registers:
# 
# zero, ra, sp, a0 - a1, a2-a4, t3-t6 and s2-s5
# The name x0 can be used in lieu of zero. Other register numbers 
# (eg. x1, x2, etc.) are not supported.
# 
# We will manually check whether you use registers properly after the ddl.
# So the grade on your autolab is not the final grade of your homework.
#======================================================================================

.data
    space:                  .asciiz  " "                # a space string.
    line:                   .asciiz "\n"                # a newline string.
    colonsp:                .asciiz ": "                # a colon string with space.
    .align                  2                           # make data aligned in word
    size:                   .word  10                   # the size of the array                           
    inputstring:            .asciiz "List need sort: "  # the origin array
    sorted_array_string:    .asciiz "Sorted:         "  # the output array
    
#=================================================     Reserved Array     ===========================================|
    #array:          .word 0 0 0 0 0 0 0 0 0 0           # array to be sorted                                        |
    array:           .word 678 567 456 765 876 987 543 -654 684 374 # use this line if you want to test this array    |
#====================================================================================================================|                                 

.text
.globl  main
main:
    j receive_values_end           # print the testing array            

    receive_values_end:
        li    a0, 4                # 4 = print_string ecall.
        la    a1, inputstring        
        ecall
        la    a1, array
        jal   print                # print user input values

    # Set up the main quick_sort call.
    # Arrays are    
    la       a1, array             # a1 adrs of the array
    li       a2, 0                 # left val (lo)
    lw       a3, size              # right val (hi)
    addi     a3, a3, -1            # make a3 the higher index
    jal      def_quick_sort  
    
    li       a0, 4                 # 4 = print_string ecall.
    la       a1, sorted_array_string   
    ecall
    la       a1, array
    jal      print                  # print out the sorted list

    j        exit


########################################################
####################your code here######################
########################################################

# In this part you will implemente quick sort and partition seperately.
# You should learn how to use stack and function call before implemente.
# WARNING: using registers properly or you will get 30% deduction after ddl.
#      50% meaningful comments is needed or you will get 50% deduction after ddl.


# Pseudocode:
#
# algorithm quicksort(A, lo, hi) is
#    if lo < hi then
#        p := partition(A, lo, hi)
#        quicksort(A, lo, p - 1 )
#        quicksort(A, p + 1, hi)
def_quick_sort:
	# store ra in stack
    sw ra, 0(sp)
    # move sp to next ptr
    addi sp, sp, -4
    
	# if lo >= hi, exit
    bge a2, a3, exit_quicksort
    
    # call partition
    jal ra, def_partition
    
    # move sp to prev ptr
    addi sp, sp, 4
    # read p from stack
    lw s2, 0(sp)
    
    # store current a3
    sw a3, 0(sp)
    # move sp to next ptr
    addi sp, sp, -4
    # a3 = p - 1
    addi a3, s2, -1
    # call quicksort
    jal ra, def_quick_sort
    # move sp to pre ptr
    addi sp, sp, 4
    # restore a3
    lw a3, 0(sp)
    
    # store current a2
    sw a2, 0(sp)
    # move sp to next ptr
    addi sp, sp, -4
    # a2 = p + 1
    addi a2, s2, 1
    # call quicksort
    jal ra, def_quick_sort
    # move sp to pre ptr
    addi sp, sp, 4
    # restore a3
    lw a2, 0(sp)
    
    # exit label
    exit_quicksort:
        # move sp to prev ptr
        addi sp, sp, 4
        # restore ra
        lw ra, 0(sp)
        # return
        jr ra


# Pseudocode:
#
# algorithm partition(A, lo, hi) is
#    pivot := A[hi]
#    i := lo - 1    
#    for j := lo to hi - 1 do
#        if A[j] < pivot then
#            i := i + 1
#            swap A[i] with A[j]
#    swap A[i + 1] with A[hi]
#    return i + 1 
def_partition:
	# length (byte) of a word
	li t3, 4
    
    # A[hi] = the value of the (hi * 4)th byte of A
	mul t4, a3, t3
    # t6 is address of A[hi]
    add t6, a1, t4
    # s2 is the pivot
    lw s2, 0(t6)
    
    # t4 is 4 * (lo-1) + address of A
    # offset bytes of a2
    mul t3, a2, t3
    # lo - 1
    addi t4, t3, -4
    # absolute address of A[i]
    add t4, t4, a1
    
    # t5 is 4 * lo + address of A
    add t5, t3, a1
    
    # start of traversal
    traversal:
    	# while j <  hi, loop
    	bge t5, t6, exit_partition

        # s3 is A[j]
        lw s3, 0(t5)
        
    	# if A[j] < pivot
        bge s3, s2, continue
        
        # i++
        addi t4, t4, 4
        # store A[i]
        lw s4, 0(t4)
        # store A[j]
        lw s5, 0(t5)
        # store A[j] in A[i]
        sw s5, 0(t4)
        # store A[i] in A[j]
        sw s4, 0(t5)
		# continue label
        continue:
            # add 4 to t5 (j++)
            addi t5, t5, 4
            # jump back
            j traversal
    # exit label
    exit_partition:
    	# i++
        addi t4, t4, 4
    	# store A[i]
        lw s4, 0(t4)
        # store A[hi]
        lw s5, 0(t6)
        # store A[hi] in A[i]
        sw s5, 0(t4)
        # store A[i] in A[hi]
        sw s4, 0(t6)
        
        # length (byte) of a word
		li t3, 4
        # index of address t4 in array
        # cut off start of A
        sub t4, t4, a1
        # divide 4 bytes
        div t4, t4, t3
        # store index in stack
        sw t4, 0(sp)
        # move sp to next ptr
        addi sp, sp, -4
        
    	# return
    	jr ra
    
    

# programs ends
# 
exit:
	# tell ecall to exit the program
	addi a0, zero, 10
    ecall                     # system call


###       Printing array
print:
    print_loop_prep:
        mv      t3, a1
        lw      t4, size
        li      t5, 0
    print_loop:
    	# if t5 == t4, end loop
    	beq t5, t4, print_end
    	# length (byte) of a word
        li t6, 4
        # offset address of current element
        mul t6, t5, t6
        # address of current element
        add t6, t6, t3
        
        # tell ecall to print int
        li a0, 1
        # set print content
        lw a1, 0(t6)
        # print
        ecall
        
        # tell ecall to print character
        li a0, 11
        # print space
        lw a1, space
        # print
        ecall
        
    	# next element
    	addi t5, t5, 1
        # continue loop
        j print_loop
    print_end:
        li      a0, 4
        la      a1, line
        ecall
        jr      ra

########################################################
####################End of your code####################
########################################################