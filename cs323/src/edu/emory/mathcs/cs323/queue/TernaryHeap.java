package edu.emory.mathcs.cs323.queue;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class TernaryHeap<T extends Comparable<T>> extends AbstractPriorityQueue<T> 
{
	private List<T> keys;
	private int size;

	@Override
	public String toString()
	{
		StringBuilder s = new StringBuilder();
		for (T key : keys)
		{
			s.append(key+"\n");
		}
		return s.toString();
	}
	
	public TernaryHeap() {
		this(Comparator.naturalOrder());
	}

	public TernaryHeap(Comparator<T> comparator) {
		super(comparator);
		keys = new ArrayList<>();
		keys.add(null); // initialize the first item as null
		size = 0;
	}

	@Override
	public int size() {
		return size;
	}

	@Override
	public void add(T key) {
		keys.add(key);
		swim(++size);
	}

	@Override
	protected T removeAux() {
		Collections.swap(keys, 1, size);
		T max = keys.remove(size--);
		sink(1);
		return max;
	}

	private void swim(int k) {
		while (1 < k && comparator.compare(keys.get((k+1)/3), keys.get(k)) < 0) {
			Collections.swap(keys, (k+1)/3, k);
			k = (k+1)/3;
		}
	}

	private void sink(int k) {
		for (int i = k * 3; i <= size; k = i, i *= 3) {
			if (i < size && comparator.compare(keys.get(i), keys.get(i+1)) < 0)
				i++;
			else if (i < size && comparator.compare(keys.get(i), keys.get(i-1)) <= 0) 
				i--;
			
			if (comparator.compare(keys.get(k), keys.get(i)) > 0)
				break;
			Collections.swap(keys, k, i);
		}
	}

}
