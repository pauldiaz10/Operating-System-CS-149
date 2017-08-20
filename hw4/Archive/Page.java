/**
 * Class representation of a page in the memory
 */
public class Page {
	int _PageNumber;
	String _ProcessName;
	int _referencedTime;
	  /**
     * Constructor that creates a page
     * @param PageNumber is the number of a page
     * @param ProcessName is the name of a process
    */
	public Page(int PageNumber, String ProcessName) {
		_PageNumber = PageNumber;
		_ProcessName = ProcessName;
		_referencedTime = 0;
	}
	/**
     * Method that sets the page number
     * @param PageNumber is the number of page to assign
     */
	public void setPageNumber (int PageNumber) {
		_PageNumber = PageNumber;
	}
	/**
     * Method that sets the name of a process
     * @param ProcessName is the name of process to assign
     */
	public void setProcessName (String ProcessName) {
		_ProcessName = ProcessName;
	}
	/**
	 * Method that counts how many times a process got referenced
	 */
	public void addReferencedTime() {
		_referencedTime++;
	}
	
	
}
