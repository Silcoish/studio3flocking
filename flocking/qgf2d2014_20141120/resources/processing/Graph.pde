import org.gicentre.utils.stat.*;

BarChart barChart;

Table table;

public class Data
{
 public Data(){}
 public Data(int rows, float l, float h)
 {
   a = new float[rows];
   low = l;
   high = h;
 }
 public float[] a;
 public float low;
 public float high; 
 void Update(Data d)
 {
  a = d.a;
  low = d.low;
  high = d.high; 
 }
}

void setup()
{
  size(700, 700);

  table = loadTable("results.csv", "header");
  println(table.getRowCount() + " total rows in table");

  float[] array = new float[table.getRowCount()];
  int  i = 0;
  
  Data lifetimeData = new Data(table.getRowCount(), 999999.0, 0);

  for (TableRow row : table.rows ())
  {
    //String prey = row.getString("Prey");
    float lifetime = row.getFloat("LifeTime");
    Data d = UpdateArray(lifetimeData, i, lifetime);
    lifetimeData.Update(d);
    //float distanceTraveled = row.getFloat("DistanceTraveled");
    //float maxVelocity = row.getFloat("MaxVelocity");
    println("Lifetime: " + lifetime);
    i++;
  }

  if ((lifetimeData.low) < 0)
    lifetimeData.low = 0;

  barChart = new BarChart(this);

  barChart.setData(lifetimeData.a);
  barChart.setMinValue(lifetimeData.low);
  barChart.setMaxValue(lifetimeData.high);
  barChart.showValueAxis(true);
  //barChart.showCategoryAxis(true);

  barChart.setBarColour(color(200, 80, 80, 150));
}

Data UpdateArray(Data d, int i, float value)
{
  d.a[i] = value;
  if (value > d.high)
    d.high = value;
  if (value < d.low)
    d.low = value;
    
   Data returnD = new Data();
   returnD.a = d.a;
   returnD.low = d.low;
   returnD.high = d.high;
   
   return d;
}

void draw()
{
  background(255);
  barChart.draw(15, 15, width/2 - 15, height/2 - 15);
  barChart.draw(width / 2 + 15, 15, width/2 - 15, height/2 - 15);
  barChart.draw(15, height / 2 + 15, width/2 - 15, height/2 - 15);
  barChart.draw(width / 2 + 15, height / 2 + 15, width/2 - 15, height/2 - 15);
}

