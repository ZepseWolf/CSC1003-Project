
/****************************************************************************************************************************************************************************/
/*Read data from input file                                                                                                                                                 */
/* Feature 1 :Season of analysis                           - Winter= -1 Spring= -0.33 Summer= 0.33 Fall= 1                                                                  */
/* Feature 2 :Age of Analysis                              - Age 18 to 36 : 0 to 1                                                                                          */
/* Feature 3 :Childish Disease                             - Yes=0, No=1                                                                                                    */
/* Feature 4 :Accident or serious trauma                   - Yes=0, No=1                                                                                                    */
/* Feature 5 :Surgical Intervention                        - Yes=0, No=1                                                                                                    */
/* Feature 6 :High fevers in last year                     - Less than three months ago= -1 More than three months ago= 0 No= 1                                             */
/* Feature 7 :Frequency of alcohol consumption             - Several times a day : 0.2 Every day : 0.4 Several times a week :0.6 Once a week :0.8 Hardly ever or never : 1  */
/* Feature 8 :Smoking Habit                                - Never : -1 Occasional : 0 Daily : 1                                                                            */
/* Feature 9 :Number of hours spent sitting per day        - Ene-16 ( 0 range to 1)                                                                                         */
/* Outcome   :Semen Diagnosis                              - Altered=1, Normal=0                                                                                            */
/****************************************************************************************************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define total_data_size 100
#define feature_size 10
#define PI 3.14159265
#define NUM_COMMANDS 7

void main();
void sortByClassification(
    int totalnum,
    int *count0,
    int *count1,
    double raw_input[total_data_size][feature_size],
    double input_0[total_data_size][feature_size],
    double input_1[total_data_size][feature_size]);

void trainFeature(
    int training_size,
    double training_input[total_data_size][feature_size],
    double calculated_training[feature_size][5]);

void testFeature(
    int classification,
    int testing_size,
    double testing_input[total_data_size][feature_size],
    double calculated_training_0[feature_size][5],
    double calculated_training_1[feature_size][5],
    double *total_error_count,
    double total_training_count_0,
    double total_training_count_1,
    int set);

double standardGaussianDis(
    double x,
    double sd,
    double mean);

void errorCal(
    int real_classification,
    double predicted_total_count_0,
    double predicted_total_count_1,
    double *total_error_count,
    int select_data_set);

int training_data_size, testing_data_size;

void plotGraph(double summary[1][5], char *label[5]);

void main()
{
  clock_t start, end;
  double cpu_time_used;
  int i, ii, total_training_count_0 = 0, total_training_count_1 = 0, total_testing_count_0 = 0, total_testing_count_1 = 0;
  double *season_of_0;
  double raw_training_input[total_data_size][feature_size];
  double raw_testing_input[total_data_size][feature_size];
  double training_input_0[total_data_size][feature_size];
  double training_input_1[total_data_size][feature_size];
  double testing_input_0[total_data_size][feature_size];
  double testing_input_1[total_data_size][feature_size];
  double calculated_training_0[feature_size][5]; //To set Conditional Probility
  double calculated_training_1[feature_size][5]; //To set Conditional Probility
  double total_error_count = 0;
  int trainingset = 0, testingset = 1;
  double summary[2][5];
  char *label[5] = {"50/50", "60/40", "70/30", "80/20", "90/10"};
  start = clock();
  for (int counter = 0; counter < 5; counter++)
  {
    switch (counter)
    {
    case 0:
      training_data_size = 50;
      testing_data_size = 50;
      printf("\n\n==========================================================");
      printf("\n\nFor 50/50 Confusion Matrix");
      break;

    case 1:
      training_data_size = 60;
      testing_data_size = 40;
      printf("\n\nFor 60/40 Confusion Matrix");
      break;

    case 2:
      training_data_size = 70;
      testing_data_size = 30;
      printf("\n\nFor 70/30 Confusion Matrix");
      break;

    case 3:
      training_data_size = 80;
      testing_data_size = 20;
      printf("\n\nFor 80/20 Confusion Matrix");
      break;

    case 4:
      training_data_size = 90;
      testing_data_size = 10;
      printf("\n\nFor 90/10 Confusion Matrix");
      break;
    }

    FILE *file_ptr;
    file_ptr = fopen("fertility_Diagnosis_Data_Group9_11.txt", "r");
    if (file_ptr == NULL)
    {
      printf("\nFile could not be opened \n");
      exit(1);
    }
    for (i = 0; i < training_data_size; i++)
    {
      fscanf(file_ptr, "%lf, %lf, %lf , %lf , %lf , %lf , %lf , %lf , %lf , %lf ", &raw_training_input[i][0], &raw_training_input[i][1], &raw_training_input[i][2], &raw_training_input[i][3], &raw_training_input[i][4], &raw_training_input[i][5], &raw_training_input[i][6], &raw_training_input[i][7], &raw_training_input[i][8], &raw_training_input[i][9]);
    }
    for (i = 0; i < testing_data_size; i++)
    {
      fscanf(file_ptr, "%lf, %lf, %lf , %lf , %lf , %lf , %lf , %lf , %lf , %lf ", &raw_testing_input[i][0], &raw_testing_input[i][1], &raw_testing_input[i][2], &raw_testing_input[i][3], &raw_testing_input[i][4], &raw_testing_input[i][5], &raw_testing_input[i][6], &raw_testing_input[i][7], &raw_testing_input[i][8], &raw_testing_input[i][9]);
    }

    fclose(file_ptr);

    sortByClassification(training_data_size, &total_training_count_0, &total_training_count_1, raw_training_input, training_input_0, training_input_1); // Output will be count0 , count1 , training_input_0, training_input_1
    sortByClassification(testing_data_size, &total_testing_count_0, &total_testing_count_1, raw_testing_input, testing_input_0, testing_input_1);       // Output will be count0 , count1 , training_input_0, training_input_1

    trainFeature(total_training_count_0, training_input_0, calculated_training_0);
    trainFeature(total_training_count_1, training_input_1, calculated_training_1);

    total_error_count = 0; //reset

    printf("\n\nUsing Testing data set:");
    testFeature(0, total_testing_count_0, testing_input_0, calculated_training_0, calculated_training_1, &total_error_count, total_training_count_0, total_training_count_1, testingset);
    testFeature(1, total_testing_count_1, testing_input_1, calculated_training_0, calculated_training_1, &total_error_count, total_training_count_0, total_training_count_1, testingset);
    printf("\n \tTotal error for testing set   : %lf %%", total_error_count * 100 / testing_data_size);
    summary[testingset][counter] = total_error_count * 100 / testing_data_size;
    total_error_count = 0; //reset

    printf("\n\nUsing Training data set:");
    testFeature(0, total_training_count_0, training_input_0, calculated_training_0, calculated_training_1, &total_error_count, total_training_count_0, total_training_count_1, trainingset);
    testFeature(1, total_training_count_1, training_input_1, calculated_training_0, calculated_training_1, &total_error_count, total_training_count_0, total_training_count_1, trainingset);

    printf("\n \tTotal error for training set   : %lf %%", total_error_count * 100 / training_data_size);
    printf("\n\n==========================================================");
    summary[trainingset][counter] = total_error_count * 100 / training_data_size;
  }

  printf("\n\nCase \t\tTraining Error \t\tTesting Error");
  for (int j = 0; j < 5; j++)
  {
    printf("\n%s", label[j]);
    printf("\t\t %lf", summary[0][j]);
    printf("\t\t %lf", summary[1][j]);
  }

  /*print graph for Testing Dataset and error probability*/
  plotGraph(summary, label);

  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("\n\nTime taken: %lf", cpu_time_used);
}

void sortByClassification(int totalnum, int *count0, int *count1, double raw_input[total_data_size][feature_size], double input_0[total_data_size][feature_size], double input_1[total_data_size][feature_size])
{
  int i = 0, ii = 0;
  *count0 = 0;
  *count1 = 0;

  for (i = 0; i < totalnum; i++)
  {
    if (raw_input[i][9] == 0)
    {
      for (ii = 0; ii < feature_size; ii++)
      {
        input_0[*count0][ii] = raw_input[i][ii];
      }
      *count0 = *count0 + 1;
    }
    else if (raw_input[i][9] == 1)
    {
      for (ii = 0; ii < feature_size; ii++)
      {
        input_1[*count1][ii] = raw_input[i][ii];
      }
      *count1 = *count1 + 1;
      ;
    }
  }
}

void trainFeature(
    int training_size,
    double training_input[total_data_size][feature_size],
    double calculated_training[feature_size][5])
{
  // Train features 1 , 3 , 4 , 5 , 6 , 7 , 8 , 2 , 9
  int i, ii;
  //Feature  1
  double f1_count1 = 0, f1_count2 = 0, f1_count3 = 0, f1_count4 = 0;
  //Feature  2  , Range
  double f2_total = 0, f2_average = 0, f2_variance = 0;
  //Feature  3
  double f3_count1 = 0, f3_count2 = 0;
  //Feature  4
  double f4_count1 = 0, f4_count2 = 0;
  //Feature  5
  double f5_count1 = 0, f5_count2 = 0;
  //Feature  6
  double f6_count1 = 0, f6_count2 = 0, f6_count3 = 0;
  //Feature  7
  double f7_count1 = 0, f7_count2 = 0, f7_count3 = 0, f7_count4 = 0, f7_count5 = 0;
  //Feature  8
  double f8_count1 = 0, f8_count2 = 0, f8_count3 = 0;
  //Feature  9  , Range
  double f9_total = 0, f9_average = 0, f9_variance = 0;

  for (i = 0; i < training_size; i++)
  {
    //Feature 1
    if (training_input[i][0] == 1)
    {
      f1_count1++;
    }
    else if (training_input[i][0] == 0.33)
    {
      f1_count2++;
    }
    else if (training_input[i][0] == -0.33)
    {
      f1_count3++;
    }
    else if (training_input[i][0] == -1)
    {
      f1_count4++;
    }

    //Feature 3
    if (training_input[i][2] == 0)
    {
      f3_count1++;
    }
    else if (training_input[i][2] == 1)
    {
      f3_count2++;
    }

    //Feature 4
    if (training_input[i][3] == 0)
    {
      f4_count1++;
    }
    else if (training_input[i][3] == 1)
    {
      f4_count2++;
    }

    //Feature 5
    if (training_input[i][4] == 0)
    {
      f5_count1++;
    }
    else if (training_input[i][4] == 1)
    {
      f5_count2++;
    }

    //Feature 6
    if (training_input[i][5] == -1)
    {
      f6_count1++;
    }
    else if (training_input[i][5] == 0)
    {
      f6_count2++;
    }
    else if (training_input[i][5] == 1)
    {
      f6_count3++;
    }

    //Feature 7
    if (training_input[i][6] == 0.2)
    {
      f7_count1++;
    }
    else if (training_input[i][6] == 0.4)
    {
      f7_count2++;
    }
    else if (training_input[i][6] == 0.6)
    {
      f7_count3++;
    }
    else if (training_input[i][6] == 0.8)
    {
      f7_count4++;
    }
    else if (training_input[i][6] == 1)
    {
      f7_count5++;
    }

    //Feature 8
    if (training_input[i][7] == -1)
    {
      f8_count1++;
    }
    else if (training_input[i][7] == 0)
    {
      f8_count2++;
    }
    else if (training_input[i][7] == 1)
    {
      f8_count3++;
    }

    //Feature 2 total sum
    f2_total += training_input[i][1];

    //Feature 9 total sum
    f9_total += training_input[i][8];
  }

  // End of loop
  // Calculate Variance
  for (i = 0; i < training_size; i++)
  {
    f2_variance += pow((training_input[i][1] - (f2_total / training_size)), 2);
    f9_variance += pow((training_input[i][8] - (f9_total / training_size)), 2);
  }

  f2_variance = sqrt(f2_variance / (training_size - 1));
  f9_variance = sqrt(f9_variance / (training_size - 1));

  //Set condition prob into calculated_training array
  calculated_training[0][0] = f1_count1 / training_size;
  calculated_training[0][1] = f1_count2 / training_size;
  calculated_training[0][2] = f1_count3 / training_size;
  calculated_training[0][3] = f1_count4 / training_size;

  calculated_training[1][0] = f2_variance;
  calculated_training[1][1] = f2_total / training_size;

  calculated_training[2][0] = f3_count1 / training_size;
  calculated_training[2][1] = f3_count2 / training_size;

  calculated_training[3][0] = f4_count1 / training_size;
  calculated_training[3][1] = f4_count2 / training_size;

  calculated_training[4][0] = f5_count1 / training_size;
  calculated_training[4][1] = f5_count2 / training_size;

  calculated_training[5][0] = f6_count1 / training_size;
  calculated_training[5][1] = f6_count2 / training_size;
  calculated_training[5][2] = f6_count3 / training_size;

  calculated_training[6][0] = f7_count1 / training_size;
  calculated_training[6][1] = f7_count2 / training_size;
  calculated_training[6][2] = f7_count3 / training_size;
  calculated_training[6][3] = f7_count4 / training_size;
  calculated_training[6][4] = f7_count5 / training_size;

  calculated_training[7][0] = f8_count1 / training_size;
  calculated_training[7][1] = f8_count2 / training_size;
  calculated_training[7][2] = f8_count3 / training_size;

  calculated_training[8][0] = f9_variance;
  calculated_training[8][1] = f9_total / training_size;
}

void testFeature(
    int classification,                                  //Actual classification of the test_input
    int testing_size,                                    //For the loop
    double testing_input[total_data_size][feature_size], // The testing data set
    double calculated_training_0[feature_size][5],       // Calculated conditional probability of 0
    double calculated_training_1[feature_size][5],       // Calculated conditional probability  of 1
    double *total_error_count,                           //Output for counting total error
    double total_training_count_0,                       // For the P(Y) of 0
    double total_training_count_1,                       // For the P(Y) of 1
    int set                                              //For error
)
{
  int i;
  double total_0, total_1, predicted_total_count_0 = 0, predicted_total_count_1 = 0;
  for (i = 0; i < testing_size; i++)
  {
    total_0 = 0;
    total_1 = 0;
    //Feature 1
    if (testing_input[i][0] == 1)
    {
      total_0 = calculated_training_0[0][0];
      total_1 = calculated_training_1[0][0];
    }
    else if (testing_input[i][0] == 0.33)
    {
      total_0 = calculated_training_0[0][1];
      total_1 = calculated_training_1[0][1];
    }
    else if (testing_input[i][0] == -0.33)
    {
      total_0 = calculated_training_0[0][2];
      total_1 = calculated_training_1[0][2];
    }
    else if (testing_input[i][0] == -1)
    {
      total_0 = calculated_training_0[0][3];
      total_1 = calculated_training_1[0][3];
    }

    //Feature 3
    if (testing_input[i][2] == 0)
    {
      total_0 *= calculated_training_0[2][0];
      total_1 *= calculated_training_1[2][0];
    }
    else if (testing_input[i][2] == 1)
    {
      total_0 *= calculated_training_0[2][1];
      total_1 *= calculated_training_1[2][1];
    }

    //Feature 4
    if (testing_input[i][3] == 0)
    {
      total_0 *= calculated_training_0[3][0];
      total_1 *= calculated_training_1[3][0];
    }
    else if (testing_input[i][3] == 1)
    {
      total_0 *= calculated_training_0[3][1];
      total_1 *= calculated_training_1[3][1];
    }

    //Feature 5
    if (testing_input[i][4] == 0)
    {
      total_0 *= calculated_training_0[4][0];
      total_1 *= calculated_training_1[4][0];
    }
    else if (testing_input[i][4] == 1)
    {
      total_0 *= calculated_training_0[4][1];
      total_1 *= calculated_training_1[4][1];
    }

    //Feature 6
    if (testing_input[i][5] == -1)
    {
      total_0 *= calculated_training_0[5][0];
      total_1 *= calculated_training_1[5][0];
    }
    else if (testing_input[i][5] == 0)
    {
      total_0 *= calculated_training_0[5][1];
      total_1 *= calculated_training_1[5][1];
    }
    else if (testing_input[i][5] == 1)
    {
      total_0 *= calculated_training_0[5][2];
      total_1 *= calculated_training_1[5][2];
    }

    //Feature 7
    if (testing_input[i][6] == 0.2)
    {
      total_0 *= calculated_training_0[6][0];
      total_1 *= calculated_training_1[6][0];
    }
    else if (testing_input[i][6] == 0.4)
    {
      total_0 *= calculated_training_0[6][1];
      total_1 *= calculated_training_1[6][1];
    }
    else if (testing_input[i][6] == 0.6)
    {
      total_0 *= calculated_training_0[6][2];
      total_1 *= calculated_training_1[6][2];
    }
    else if (testing_input[i][6] == 0.8)
    {
      total_0 *= calculated_training_0[6][3];
      total_1 *= calculated_training_1[6][3];
    }
    else if (testing_input[i][6] == 1)
    {
      total_0 *= calculated_training_0[6][4];
      total_1 *= calculated_training_1[6][4];
    }

    //Feature 8
    if (testing_input[i][7] == -1)
    {
      total_0 *= calculated_training_0[7][0];
      total_1 *= calculated_training_1[7][0];
    }
    else if (testing_input[i][7] == 0)
    {
      total_0 *= calculated_training_0[7][1];
      total_1 *= calculated_training_1[7][1];
    }
    else if (testing_input[i][7] == 1)
    {
      total_0 *= calculated_training_0[7][2];
      total_1 *= calculated_training_1[7][2];
    }

    //Feature 2
    total_0 *= standardGaussianDis(testing_input[i][1], calculated_training_0[1][0], calculated_training_0[1][1]);
    total_1 *= standardGaussianDis(testing_input[i][1], calculated_training_1[1][0], calculated_training_1[1][1]);
    //Feature 9
    total_0 *= standardGaussianDis(testing_input[i][8], calculated_training_0[8][0], calculated_training_0[8][1]);
    total_1 *= standardGaussianDis(testing_input[i][8], calculated_training_1[8][0], calculated_training_1[8][1]);
    // Mutiply total count prob
    total_0 *= (total_training_count_0 / training_data_size);
    total_1 *= (total_training_count_1 / training_data_size);
    if (total_0 > total_1)
    {
      //Predicted 0
      predicted_total_count_0++;
    }
    else
    {
      //Predicted 1
      predicted_total_count_1++;
    }
    //End of loop
  }
  errorCal(classification, predicted_total_count_0, predicted_total_count_1, total_error_count, set);
}

double standardGaussianDis(double x, double sd, double mean)
{
  double cal = 0;
  cal = expl((pow((x - mean) / sd, 2)) / -2) / sqrt(2 * PI);
  return cal;
}

void errorCal(
    int real_classification,        // Actual classification
    double predicted_total_count_0, // Total predicted count of 0
    double predicted_total_count_1, // Total predicted count of 1
    double *total_error_count,      // Output for counting total error
    int select_data_set             // selected data set for total size
)
{

  double size;
  if (select_data_set == 0)
  {
    size = training_data_size;
  }
  if (select_data_set == 1)
  {
    size = testing_data_size;
  }
  // Real vs predicted , recorded down
  if (real_classification == 0)
  {
    *total_error_count += predicted_total_count_1;
    printf("\n \tTrue negatives  : %lf %%", predicted_total_count_0 * 100 / size);
    printf("\n \tFalse positives : %lf %%", predicted_total_count_1 * 100 / size);
  }
  else if (real_classification == 1)
  {
    *total_error_count += predicted_total_count_0;
    printf("\n \tTrue positives  : %lf %%", predicted_total_count_1 * 100 / size);
    printf("\n \tFalse negatives : %lf %%", predicted_total_count_0 * 100 / size);
  }
  else
  {
    printf("\n \tNo such classification exist!");
  }
};

void plotGraph(double summary[1][5], char *label[5])
{
  char *commandsForGnuplot[] = {                                                                                                                                                             // Array for gnuplot commands
                                "set title \"Error Probability\"",                                                                                                                           // Graph title
                                "set xrange [40:100]",                                                                                                                                       // x-axis range from 40 to 100
                                "set yrange [0:30]",                                                                                                                                         // y-axis range from 0 to 30
                                "set xlabel \"Number of Test Data (Training:Testing)\"",                                                                                                     // x-axis labels
                                "set ylabel \"Error Probability in Percentage\"",                                                                                                            //y-axis labels
                                "set xtics ('50:50' 50, '60:40' 60,  '70:30' 70, '80:20' 80 , '90:10' 90)",                                                                                  //Assign x-axis labels to 50:50 to 90:10
                                "plot '$testing' title 'Testing line' linecolor 24 linewidth 2 smooth csplines, '$training' title 'Training line' linecolor 7 linewidth 2 smooth csplines"}; // call gnuplot function to plot graph
  FILE *gnuPlot = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent", "w");                                                                                                                 // to amend the file location as to where your gnuplot.exe lies

  fprintf(gnuPlot, "$testing << EOD \n"); //Create a data file for testing set
  for (int i = 0; i < 5; i++)
  {
    fprintf(gnuPlot, "%s %f\n", label[i], summary[1][i]);
  }
  fprintf(gnuPlot, "EOD\n"); // End of data feeding to the testing set data file

  fprintf(gnuPlot, "$training << EOD \n"); //Create a data file for training set
  for (int i = 0; i < 5; i++)
  {
    fprintf(gnuPlot, "%s %f\n", label[i], summary[0][i]);
  }
  fprintf(gnuPlot, "EOD\n"); // End of data feeding to the training set data file

  for (int i = 0; i < NUM_COMMANDS; i++)
  {
    fprintf(gnuPlot, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
  }

  fclose(gnuPlot);
}
