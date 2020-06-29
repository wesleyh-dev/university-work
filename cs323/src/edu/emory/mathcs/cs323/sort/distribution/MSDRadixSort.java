/** THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon **/

package edu.emory.mathcs.cs323.sort.distribution;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import edu.emory.mathcs.cs323.utils.DSUtils;

public class MSDRadixSort<T extends Comparable<T>> extends BucketSort<Integer> {
	private final int MAX;
	private int div;
	private int d;
	private List<Integer>[] buckets;

	public MSDRadixSort(int maxDigits) {
		this(maxDigits, Comparator.naturalOrder());
	}

	public MSDRadixSort(int maxDigits, Comparator<Integer> comparator) {
		super(10, true, comparator);
		MAX = maxDigits;
		div = (int) Math.pow(10, MAX);
	}

	@SuppressWarnings("unchecked")
	@Override
	public void sort(Integer[] array, int beginIndex, int endIndex) {
		buckets = (List<Integer>[]) DSUtils.createEmptyListArray(10);

		// Sort array into buckets
		for (int i = beginIndex; i < endIndex; i++) {
			buckets[getBucketIndex(array[i])].add(array[i]);
		}

		// // Effectively repeat previous loop for next MSD for each bucket
		// (which is effectively an array)
		// for (List<Integer> bucket : buckets)
		// {
		// d = div/10;
		// // Skip if bucket is empty or contains 1 element
		// if (bucket.isEmpty()) continue;
		// int v = 0;
		// int x = d;
		// List<Integer> replace = new ArrayList<Integer>();
		// // For each set of elements sorted by MSD, perform a sort of those
		// elements separately
		// while (x>=1)
		// {
		// List<Integer> n = new ArrayList<Integer>();
		// for (int i = beginIndex; i < endIndex; i++)
		// {
		// if ((bucket.get(i)/x)%10 != v) continue;
		// else n.add(bucket.get(i));
		// }
		// n = replaceSort(n, beginIndex, n.size(), x);
		// v++;
		// replace.addAll(n);
		// x/=10;
		// }
		// }
		//
		//
		// // Make final sorted array
		// for (List<Integer> bucket : buckets)
		// {
		// for (Integer key : bucket) array[beginIndex++] = key;
		// bucket.clear();
		// }

		for (List<Integer> bucket : buckets) {
			Collections.sort(bucket);
			for (Integer key : bucket)
				array[beginIndex++] = key;
			bucket.clear();
		}
	}

	// public List<Integer> replaceSort(List<Integer> bucket, int begin, int
	// end, int d)
	// {
	// // New list of buckets
	// List<Integer>[] newbuckets =
	// (List<Integer>[])DSUtils.createEmptyListArray(10);
	//
	// // List to replace current passed bucket
	// List<Integer> ret = new ArrayList<Integer>();
	//
	// // Add each element to a bucket
	// for (int i = begin; i < end; i++)
	// {
	// newbuckets[(bucket.get(i)/d)%10].add(bucket.get(i));
	// }
	//
	// // Concatenate each bucket into one List @ret to replace passed bucket
	// for (List<Integer> buck : newbuckets)
	// {
	// if (buck.isEmpty()) continue;
	// ret.addAll(buck);
	// buck.clear();
	// }
	// return ret;
	// }

	@Override
	protected int getBucketIndex(Integer key) {
		int b = (key / div) % 10;
		return b;
	}

}
