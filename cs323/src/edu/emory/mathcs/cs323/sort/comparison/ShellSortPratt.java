/** THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon **/

package edu.emory.mathcs.cs323.sort.comparison;

import java.util.Collections;
import java.util.Comparator;

public class ShellSortPratt<T extends Comparable<T>> extends ShellSort<T> {
	public ShellSortPratt() {
		this(Comparator.naturalOrder());
	}

	public ShellSortPratt(Comparator<T> comparator) {
		this(comparator, 1000);
	}

	public ShellSortPratt(Comparator<T> comparator, int n) {
		super(comparator, n);
	}

	@Override
	protected void populateSequence(int n) {
		n /= 3;
		int q = 0;
		int pow3 = (int) Math.pow(3, q);
		while (pow3 <= n) {
			int pow3x2 = pow3;
			while (pow3x2 <= n) {
				sequence.add(pow3x2);
				pow3x2 = pow3x2 * 2;
			}
			pow3 = pow3 * 3;
		}
		Collections.sort(sequence);
	}

	@Override
	protected int getSequenceStartIndex(int n) {
		int index = Collections.binarySearch(sequence, n / 3);
		if (index < 0)
			index = -(index + 1);
		if (index == sequence.size())
			index--;
		return index;
	}

}
