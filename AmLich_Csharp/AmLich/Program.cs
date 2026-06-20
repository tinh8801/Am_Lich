/*
 * Created by SharpDevelop.
 * User: Huyth
 * Date: 06/08/2026
 * Time: 12:42 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Text;
namespace AmLich
{
	class Program
	{
		public static void Main(string[] args)
		{
			// TODO: Implement Functionality Here
			Console.OutputEncoding = Encoding.UTF8;
			int p1, p2, p3;
			if (args.Length==3){
				if (int.TryParse(args[0], out p1) && int.TryParse(args[1], out p2) && int.TryParse(args[2], out p3)){
					if((p1>0 && p1<=31) && (p2>0 && p2<=12) && (p3>0 && p3<=2050)){
					   	DateTime d = new DateTime(p3, p2, p1);
					   	Console.WriteLine(LunarYearTools.SolarToLunar(d).ToString());
					}else{
						Console.WriteLine("Invalid Args");
					}
				}
			}else{
				Console.WriteLine("Too Few Args");
			}
			DateTime sd = new DateTime(2025, 7, 25);
			LunarDate ld = LunarYearTools.SolarToLunar(sd);
			Console.WriteLine("Ngày Dương Lịch: " + sd.ToString());
			Console.WriteLine("==>Ngày Âm Lịch: "+ld.ToString());
			ld = new LunarDate(1, 2, 2023, true);
			sd = LunarYearTools.LunarToSolar(ld);
			Console.WriteLine("Ngày Âm Lịch: "+ld.ToString());
			Console.WriteLine("==>Ngày Dương Lịch: : "+sd.ToString());
			sd = new DateTime(2023, 1, 21);
			ld = LunarYearTools.SolarToLunar(sd);
			Console.WriteLine("Ngày Dương Lịch: " + sd.ToString());
			Console.WriteLine("==>Ngày Âm Lịch: "+ld.ToString());
			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
		}
	}
}