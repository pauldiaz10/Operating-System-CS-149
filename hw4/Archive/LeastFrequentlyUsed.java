/**
 * Class that simulates LeastFrequenltyUsed Algorithm
 */
import java.util.LinkedList;

public class LeastFrequentlyUsed implements ReplacementAlgorithm {	
	String _name;
	/**
	 * Constructor that creates the class LeastRecentlyUsed 
	 */
	public LeastFrequentlyUsed (String name) {
		_name = name;
	}
	/**
	 * Constructor that creates the class LeastFrequentlyUsed 
	 */
	@Override
	public String getName() {
		return _name;
	}
	/**
	 * Method that runs LeastFrequenlyUsed, and figure out the page
	 * which has the least frequently used page
     * @param replacement represents the main memory
	 * @PageQueu contains the list of processes according to their arrival time
	 * @return the page name
	 */
	@Override
	public String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue) {
		
		String evicted;
		int min = PageQueue.get(0)._referencedTime;				//first we find the process that had minimum
		int index = 0;											//process least amount of references
		for (int i = 1; i < PageQueue.size(); i++) {			//we, then go through the list and compare
			if (min > PageQueue.get(i)._referencedTime) {		//this process with list of processes we have
				min = PageQueue.get(i)._referencedTime;
				index = i;										//here, now we kno which one is the least 
			}
		}	
		evicted = PageQueue.get(index)._ProcessName;			//store the name of the process
		int num = PageQueue.get(index)._PageNumber;				//as well as the page number
		PageQueue.remove(index);
		
		for (int i = 0 ; i < replacementList.size() ; i++) {
			if (replacementList.get(i)._PageNumber == num &&
				replacementList.get(i)._ProcessName.equals(evicted)) {
				replacementList.set(i, new Page(-1, "."));
				break;
			}
		}
		
		return evicted;
	}

}
