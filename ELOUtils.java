import java.math.BigDecimal;
 
import ELOResult;
 
public class ELOUtils {
 
	private final static BigDecimal DONE = new BigDecimal("1.0");
	private final static BigDecimal D400 = new BigDecimal("400.0");
	
	
	/**
	 * 通过ELO算法计算竞技得分
	 * @param ra		玩家A本轮竞技前得分
	 * @param rb		玩家B本轮竞技前得分
	 * @param sa		A vs B 结果：<PRE>胜利	1<br>平	0.5<br/>失败	0</PRE>
	 * @param k			极限值，代表理论上最多可以赢一个玩家的得分和失分
	 * @param limit		是否开启下限为0限制
	 * @return			本轮结束后A、B玩家得分
	 */
	public static ELOResult rating(int ra, int rb, float sa, int k, boolean limit){
		
		// 统一由分数高的一方计算出积分变动情况
		if(ra < rb){
			ELOResult result = rating(rb, ra, 1.0f-sa, k, limit);
			int temp = result.getRa();
			result.setRa(result.getRb());
			result.setRb(temp);
			return result;
		}
		
		BigDecimal ea = DONE.divide(DONE.add(new BigDecimal(Math.pow(10, new BigDecimal(rb - ra).divide(D400, 6, BigDecimal.ROUND_HALF_UP).doubleValue()))), 6, BigDecimal.ROUND_HALF_UP);
		double score = new BigDecimal(k).multiply(new BigDecimal(sa).subtract(ea)).doubleValue();
		
		// 为正数变动积分向上取整
		int scoreI = (int) Math.ceil(score);
		if(score < 0d){		// 为负数变动积分向下取整
			scoreI = (int) Math.floor(score);
		}
		
		ELOResult elo = new ELOResult();
		elo.setRa((ra + scoreI < 0 && limit) ? 0 : ra + scoreI);
		elo.setRb((rb - scoreI < 0 && limit) ? 0 : rb - scoreI);
		
		return elo;
		
	}
	
}