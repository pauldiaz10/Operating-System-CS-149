
public class Process {
	public Process(String name, float arrivalTime, float expectedTime, int priority){
		setId(name);
		setTimeArrival(arrivalTime);
		setTotalExpectedRun(expectedTime);
		setPriority(priority);
		setStartedProcess(false);
		setTimeRemaining(expectedTime);
	}
	public String getId() {
		return id + " ";
	}
	public void setId(String id) {
		this.id = id;
	}
	public float getTimeArrival() {
		return timeArrival;
	}
	public void setTimeArrival(float timeArrival) {
		this.timeArrival = timeArrival;
	}
	public float getTotalExpectedRun() {
		return totalExpectedRun;
	}
	public void setTotalExpectedRun(float totalExpectedRun) {
		this.totalExpectedRun = totalExpectedRun;
	}
	public int getPriority() {
		return priority;
	}
	public void setPriority(int priority) {
		this.priority = priority;
	}
	public int getTimeStart() {
		return timeStart;
	}
	public void setTimeStart(int timeStart) {
		this.timeStart = timeStart;
		setStartedProcess(true);
	}
	public int getTimeFinish() {
		return timeFinish;
	}
	public void setTimeFinish(int timeFinish) {
		this.timeFinish = timeFinish;
	}
	public int getTimeWaiting() {
		return timeWaiting;
	}
	public void setTimeWaiting(int timeWaiting) {
		this.timeWaiting = timeWaiting;
	}
	public float getTimeRemaining() {
		return timeRemaining;
	}
	public void setTimeRemaining(float timeRemaining) {
		this.timeRemaining = timeRemaining;
	}
	public boolean isStartedProcess() {
		return startedProcess;
	}
	public void setStartedProcess(boolean startedProcess) {
		this.startedProcess = startedProcess;
	}
	public float getTimeToFinish() {
		return timeToFinish;
	}
	public void setTimeToFinish(float timeToFinish) {
		this.timeToFinish = timeToFinish;
	}
	public boolean isStarted() {
		return started;
	}
	public void setStarted(boolean started) {
		this.started = started;
	}
	public Process getNext() {
		return next;
	}
	public void setNext(Process next) {
		this.next = next;
	}
    public float getWaiting() 
	{
        float waitingTime = timeStart - timeArrival; 
        return waitingTime; 
    }
    public float getTurnaround() 
	{
        return (float) timeFinish - timeArrival;
    }
    public int getResponse() 
	{
        int responseTime = timeFinish - timeStart; 

        return responseTime;
    }
    public void runProcess() 
	{
        timeRemaining -= 1;
    }

	private String id;
	private float timeArrival;
	private float totalExpectedRun;
	private int priority;
	private int timeStart;
	private int timeFinish;
	private int timeWaiting;
	private float timeRemaining;
	private boolean startedProcess;
	private float timeToFinish;
	private boolean started;
	private Process next; //like a linked list
	
	public String toString(){
		return "Process " + getId() + " | Arrival Time: " + getTimeArrival() + " | Expected Run: " + getTotalExpectedRun() + " | Priority " + getPriority();
	}
}
