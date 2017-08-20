/**
 * Class that simulates LeastRecentlyUsed Algorithm
 */
import java.util.LinkedList;

public class LeastRecentlyUsed implements ReplacementAlgorithm{
	
	String _name;
	/**
	 * Constructor that creates the class LeastRecentlyUsed 
	 */
	public LeastRecentlyUsed(String name) {
		_name = name;
	}
	/**
	 * Method that runs LeastRecentlyUsed, and figure out the page
	 * which has the least recently used page
     * @param replacement represents the main memory
	 * @PageQueu contains the list of processes according to their arrival time
	 * @return the page name
	 */
	@Override
	public String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue) {
		String evict;
		Page p = PageQueue.poll();		//we extract the page that's in the list of processes that we have
		evict = p._ProcessName;			//and get the name of it
		
		for (int i = 0 ; i < replacementList.size() ; i++) {					//we go through the list and
			if (replacementList.get(i)._PageNumber == p._PageNumber &&			//find that page, we find it in 
				replacementList.get(i)._ProcessName.equals(p._ProcessName)) {	//replacementList, then we are done
				replacementList.set(i, new Page(-1, "."));
				break;
			}
		}	
		return evict;
	}
	/**
	 * Method that gets the name of the algorithm
	 * @return string name of the algorithm
	 */
	@Override
	public String getName() {
		return _name;
	}

}
