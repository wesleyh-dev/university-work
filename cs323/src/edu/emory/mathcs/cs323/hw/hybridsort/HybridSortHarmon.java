/** THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon **/

package edu.emory.mathcs.cs323.hw.hybridsort;

import java.lang.reflect.Array;
import java.util.Arrays;

import edu.emory.mathcs.cs323.sort.AbstractSort;
import edu.emory.mathcs.cs323.sort.comparison.HeapSort;
import edu.emory.mathcs.cs323.sort.comparison.InsertionSort;
import edu.emory.mathcs.cs323.sort.comparison.ShellSortKnuth;
import edu.emory.mathcs.cs323.sort.divide_conquer.IntroSort;

public class HybridSortHarmon<T extends Comparable<T>> implements HybridSort<T> {
	private AbstractSort<T> engine;
	private int[] starts;

	@Override
	@SuppressWarnings("unchecked")
	public T[] sort(T[][] input) {
		// Sort each row separately
		// Determine which sort to use for optimizing time usage
		int i = 0;
		for (T[] row : input) {
			boolean asc = isAscending(row);
			boolean des = isDescending(row);
			if (row.length < 2)
				continue;
			if (asc == true) {
				engine = new InsertionSort<>();
				engine.sort(row, 0, row.length);
			} else if (des == true) {
				engine = new ShellSortKnuth<>();
				engine.sort(row, 0, row.length);
			} else {
				engine = new HeapSort<>();
				engine.sort(row, 0, row.length);
			}
		}

		// Place all sorted rows into 1-D array
		int size = Arrays.stream(input).mapToInt(t -> t.length).sum();
		T[] output = (T[]) Array.newInstance(input[0][0].getClass(), size);
		int beginIndex = 0;
		for (T[] t : input) {
			System.arraycopy(t, 0, output, beginIndex, t.length);
			beginIndex += t.length;
		}

		AbstractSort<T> finEngine = new HeapSort<>();
		AbstractSort<T> intro = new IntroSort<>(finEngine);
		intro.sort(output, 0, size);
		return output;
	}

	// Check if row is ascending and sorted or mostly sorted
	public boolean isAscending(T[] row) {
		int count = 0;
		for (int i = 0; i < row.length - 1; i++) {
			if (row[i].compareTo(row[i + 1]) < 0)
				count++;
		}
		if (count > row.length - count)
			return true;
		else
			return false;
	}

	// Check if row is in descending order or mostly descending
	public boolean isDescending(T[] row) {
		int count = 0;
		for (int i = 0; i < row.length - 1; i++) {
			if (row[i].compareTo(row[i + 1]) >= 0)
				count++;
		}
		if (count > row.length - count)
			return true;
		else
			return false;
	}

}
