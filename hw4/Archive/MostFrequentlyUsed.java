
/**
 * Class that simulates MostFequentlyUsed Algorithm
*/
import java.util.LinkedList;

public class MostFrequentlyUsed implements ReplacementAlgorithm{

	String _name;
	
	/**
	 * Constructor that creates the class MostFrequentlyUsed
	 */
	public MostFrequentlyUsed (String name) {
		_name = name;
	}
	/**
	 * Method that gets the name of the algorithm
	 * @return string name of the algorithm
	 */
	@Override
	public String getName() {
		
		return _name;
	}
	/**
	 * Method that runs MostFrequentlyUsed, and figure out the page
	 * which has the most frequently used page
	 * @return the process name of got evicted, which is the 
	 * @param replacement represents the main memory
	 * @PageQueu contains the list of processes according to their arrival time
	 */
	@Override
	public String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue) {
		String evicted;
		int max = PageQueue.get(0)._referencedTime;
		int index = 0;
		for (int i = 1; i < PageQueue.size(); i++) {			//go through the list and compare
			if (max < PageQueue.get(i)._referencedTime) {       //page that has the maximum reference
				max = PageQueue.get(i)._referencedTime;
				index = i;										//here we found it
			}
		}
		
		evicted = PageQueue.get(index)._ProcessName;			//we get the name of that process
		int num = PageQueue.get(index)._PageNumber;				//we get the page number of that process
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
