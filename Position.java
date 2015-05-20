
public class Position implements Comparable<Position>{
	int cells_player[] = new int[6];
	int cells_computer[] = new int[6];
	int seeds_player;
	int seeds_computer;
	boolean computer_play;
	
    public Position(){
    	seeds_player = 0;
    	seeds_computer = 0;
    }
    public Position(int a1[], int a2[], int s1, int s2)
    {
            cells_player = a1;
            cells_computer = a2;
            seeds_player = s1;
            seeds_computer = s2;
    }
   
    @Override
    public boolean equals(Object a)
    {
//    		System.out.println("equals");
            if(!(a instanceof Position))
                    return false;
            Position t = (Position) a;
            
//            if(t.computer_play != computer_play)
//            	return false;
            
            if(t.computer_play == computer_play &&
				t.cells_player[0] == cells_player[0] &&
	            t.cells_player[1] == cells_player[1] &&
	            t.cells_player[2] == cells_player[2] &&
	            t.cells_player[3] == cells_player[3] &&
	            t.cells_player[4] == cells_player[4] &&
	            t.cells_player[5] == cells_player[5] &&
	            t.cells_computer[0] == cells_computer[0] &&
	            t.cells_computer[1] == cells_computer[1] &&
	            t.cells_computer[2] == cells_computer[2] &&
	            t.cells_computer[3] == cells_computer[3] &&
	            t.cells_computer[4] == cells_computer[4] &&
	            t.cells_computer[5] == cells_computer[5] &&
	            t.seeds_player == seeds_player &&
	            t.seeds_computer == seeds_computer)
                    return true;
           
            return false;
    }
   
//    @Override
//    public int hashCode()
//    {
//    	StringBuilder sb = new StringBuilder();
//    	
//    	sb.append(cells_player[0]);
//        sb.append(cells_player[1]);
//        sb.append(cells_player[2]);
//        sb.append(cells_player[3]);
//        sb.append(cells_player[4]);
//        sb.append(cells_player[5]);
//        
//        sb.append(cells_computer[0]);
//        sb.append(cells_computer[1]);
//        sb.append(cells_computer[2]);
//        sb.append(cells_computer[3]);
//        sb.append(cells_computer[4]);
//        sb.append(cells_computer[5]);
//        
//        sb.append(seeds_player);
//        sb.append(seeds_computer);
//        
//        sb.append(computer_play);
//        
//        return sb.toString().hashCode();
//    }
   
    @Override
    public String toString()
    {
            String s = "";
            for(int a : cells_player)
                    s += a + " ";
            s += "\n";
            for(int a : cells_computer)
                    s += a + " ";
            s += "\n" + seeds_player + " " + seeds_computer;
            return s;
    }
    public int compareTo(Position o) {

		 return (10*o.cells_player[0] - 10*cells_player[0] +
               100*o.cells_player[1] - 100*cells_player[1] +
               1000*o.cells_player[2] - 1000*cells_player[2] +
               10000*o.cells_player[3] - 10000*cells_player[3] +
               899*o.cells_player[4] - 899*cells_player[4] +
               7*o.cells_player[5] - 7*cells_player[5] +
               (-998)*o.cells_computer[0] - (-998)*cells_computer[0] +
               3*o.cells_computer[1] - 3*cells_computer[1] +
               456*o.cells_computer[2] - 456*cells_computer[2] +
               789*o.cells_computer[3] - 789*cells_computer[3] +
               11235*o.cells_computer[4] - 11235*cells_computer[4] +
               547*o.cells_computer[5] - 547*cells_computer[5] +
               109*o.seeds_player - 109*seeds_player +
               1090*o.seeds_computer - 1090*seeds_computer);		
	}
    
//	@Override
//	public int compareTo(Position o) {
//		
//		if(o.equals(this)){
//			return 0;
//		}
//		int sum = 0;
//		for (int i : cells_computer) {
//			sum += i;
//		}
//		int sum1 = 0;
//		for (int i : cells_player) {
//			sum1 += i;
//		}
//		return sum - sum1;
//	}
}
