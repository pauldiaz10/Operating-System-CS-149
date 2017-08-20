import java.util.ArrayList;

public interface Schedule {
	public String schedule(ArrayList<Process> a);
	


	public float averageTurnaroundTime();

	public float averageWaitingTime();

	public float averageResponseTime();

	public float totalThroughput();
}
