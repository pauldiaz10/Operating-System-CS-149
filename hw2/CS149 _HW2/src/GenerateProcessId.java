import java.util.HashMap;
public class GenerateProcessId {
	public static String toAlphabetic(int i) {
	    if( i<0 ) {
	        return "-"+toAlphabetic(-i-1);
	    }

	    int quot = i/26;
	    int rem = i%26;
	    char letter = (char)((int)'A' + rem);
	    if( quot == 0 ) {
	        return ""+letter;
	    } else {
	        return toAlphabetic(quot-1) + letter;
	    }
	}
	public static int toNumber(String s) {
	    if(s==null || s.length() == 0){
	        throw new IllegalArgumentException("Input is not valid!");
	    }
	 
	    HashMap<Character, Integer> map = new HashMap<Character, Integer>();
	    char c = 'A';
	    for(int i=1; i<=26; i++){
	        map.put(c, i);
	        c += 1; 
	    }
	 
	    int result = 0;
	    int i = s.length()-1;
	    int t = 0;
	    while(i >= 0){
	        char curr = s.charAt(i);
	        result = result + (int) Math.pow(26, t) * map.get(curr);
	        t++;
	        i--;
	    }
	 
	    return result;
	}
}
