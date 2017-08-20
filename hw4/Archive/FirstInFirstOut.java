/**
 * Class that simulates First in First out Algorithm
*/
import java.util.LinkedList;

public class FirstInFirstOut implements ReplacementAlgorithm {
	
	String _name;
	/**
	 * Constructor that creates the class First in first out
	 */
	public FirstInFirstOut(String name) {
		_name = name;
	}
	/**
	 * Method that runs First in first out, and figure out the page
	 * that arrived first, then that's the page that we are going to evict
	 * @return the process name of got evicted, which is the 
	 * @param replacement represents the main memory
	 * @PageQueu contains the list of processes according to their arrival time
	 */
	@Override
	public String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue) {
		
		String evict;
		Page p = PageQueue.poll();
		evict = p._ProcessName;
		
		for (int i = 0 ; i < replacementList.size() ; i++) {					//we go though the list and 
			if (replacementList.get(i)._PageNumber == p._PageNumber &&			//compare and find that process
				replacementList.get(i)._ProcessName.equals(p._ProcessName)) {	//if we do, then we are done
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
	}w
}
