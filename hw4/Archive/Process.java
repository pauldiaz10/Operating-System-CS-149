/*Group #3
Group Members:
Yulong Chen (ID: 0101474984)
Paul Diaz (ID: 010188451)
Haokai Ma (ID: 010473164)
Rong Wang (ID: 010359492)
Long Trinh (ID: 010174476)
*/
/**
 * Class representation of a process
 */
import java.util.LinkedList;

public class Process {
	private int _arrivalTime;			//time a process arrives
	private int _size;					//size it needs to be stored in
	private double _durations;			//time it takes to finish its task
	private String _name;				//name of a process
	private int _currentReferences;		//number of the current process reference
	
	/**
     * Constructor that creates a process
     * @param arrivalTime is the time a process arrives
     * @param size is the size of a process needs to be stored in the memory
     * @param durations is the time it takes ti finish its task
     */
	public Process (int arrivalTime, int size, double durations) {
		_arrivalTime = arrivalTime;
		_size = size;
		_durations = durations;
		_currentReferences = 0;
	}
	/**
     * Method that returns the current process reference
     * @return the number where the current process reference
    */
	public int getCurrentReferences() {
		return _currentReferences;
	}
	/**
     * Method that sets the current process reference
     * @param currentReferences is the number of new current reference
     */
	public void setCurrentReferences(int currentReferences) {
		_currentReferences = currentReferences;
	}
	/**
     * Method that returns the arrival time of a process
     * @return the number of arrival of time of a process
     */
	public int getArrivalTime() {
		return _arrivalTime;
	}
	/**
     * Method that sets the arrival time of a process
     * @param _arrivalTime is the new arrival time of a process
     */
	public void setArrivalTime(int _arrivalTime) {
		this._arrivalTime = _arrivalTime;
	}
    /**
     * Method that returns the size of a process needs to be stored in the memory
     * @return the size of the process
     */
	public int getSize() {
		return _size;
	}
    /**
     * Method that sets the size of a process
     * @param _size is the new size of a process
     */
	public void setSize(int _size) {
		this._size = _size;
	}
    /**
     * Method that gets the duration time it takes for a process to do its task
     * @return the number of time it take a process to finish its task
     */
	public double getDurations() {
		return _durations;
	}
    /**
     * Method that sets the time of duration of a process to finish its task
     * @param _durations is the time needed for a process to finish its task
     */
	public void setDurations(double _durations) {
		this._durations = _durations;
	}
    /**
     * Method that gets the name of a process
     * @return the name of a process
     */
	public String getName() {
		return _name;
	}
    /**
     * Method that sets the name of a process
     * @param _name is the new name of a process
     */
	public void setName(String _name) {
		this._name = _name;
	}
	
	
	
}
