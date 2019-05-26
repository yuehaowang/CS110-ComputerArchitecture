#!/bin/bash
a=(82 14 83 16 80 37 47 87 12 79 9 39 32 99 67 89 54 4 6 12)

quick_sort(){
	# The entrance function to sort the array `a` from the small to the big
	# -- your codes begin
	# TODO: fill your codes here
	partition(){
		pivot=${a[$2]}
		i=$1
		j=$1

		while (( $j<$2 ))
		do
			if (( ${a[$j]} < $pivot ))
			then
				temp=${a[$i]}
				a[$i]=${a[$j]}
				a[$j]=$temp

				i=$(( $i + 1 ))
			fi

			j=$(( $j + 1 ))
		done

		a[$2]=${a[$i]}
		a[$i]=$pivot

		return $i
	}

	quick_sort_part(){
		if (( $1 >= $2 ))
		then
			return
		fi

		partition $1 $2
		p=$?
		quick_sort_part $1 $(( $p - 1 ))
		quick_sort_part $(( $p + 1 )) $2
	}

	quick_sort_part 0 19

	echo 'Result: '${a[*]}

	# -- your codes end
	# Any modification in other lines is NOT allowed.
	echo
}

# invoke the sorting function
quick_sort

# check the correctness
ans=(4 6 9 12 12 14 16 32 37 39 47 54 67 79 80 82 83 87 89 99)
for((i=0;i<20;i++))
{
	if [ "${a[$i]}" -ne "${ans[$i]}" ];
	then
		echo "Wrong answer at position $i"
		exit 127
	fi
}
echo 'Correct!'