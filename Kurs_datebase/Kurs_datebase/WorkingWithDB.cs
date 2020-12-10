using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Kurs_datebase
{
    public partial class WorkingWithDB : Form
    {
        SqlConnection sqlConnection;
        public WorkingWithDB()
        {
            InitializeComponent();
        }

        private async void WorkingWithDB_Load(object sender, EventArgs e)
        {
            string connectionString = @"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=C:\Users\111\source\repos\storage\Kurs_datebase\Kurs_datebase\Database.mdf;Integrated Security=True";

            sqlConnection = new SqlConnection(connectionString);

            await sqlConnection.OpenAsync();

            SqlDataReader sqlReader = null;

            SqlCommand command = new SqlCommand("SELECT * FROM [Products]", sqlConnection);

            try
            {
                sqlReader = await command.ExecuteReaderAsync();

               while(await sqlReader.ReadAsync())
                {
                    listBox1.Items.Add(Convert.ToString(sqlReader["Id"]) + "   " + Convert.ToString(sqlReader["Name"])
                        + "   " + Convert.ToString(sqlReader["Prise"]) + "   " + Convert.ToString(sqlReader["Weight"]) + "   " + Convert.ToString(sqlReader["Date"])
                        + "   " + Convert.ToString(sqlReader["Provider"]) + "   " + Convert.ToString(sqlReader["Descriptoin"]));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), ex.Source.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (sqlReader != null)
                    sqlReader.Close();
            }

        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (sqlConnection != null && sqlConnection.State != ConnectionState.Closed)
                sqlConnection.Close();
        }

        private void WorkingWithDB_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (sqlConnection != null && sqlConnection.State != ConnectionState.Closed)
                sqlConnection.Close();
        }

        private async void button1_Click(object sender, EventArgs e)
        {
            if (label15.Visible)
                label15.Visible = false;

            if (!string.IsNullOrEmpty(textBox1.Text) && !string.IsNullOrWhiteSpace(textBox1.Text) &&
                !string.IsNullOrEmpty(textBox2.Text) && !string.IsNullOrWhiteSpace(textBox2.Text) &&
                !string.IsNullOrEmpty(textBox4.Text) && !string.IsNullOrWhiteSpace(textBox4.Text) &&
                !string.IsNullOrEmpty(textBox3.Text) && !string.IsNullOrWhiteSpace(textBox3.Text) &&
                !string.IsNullOrEmpty(textBox6.Text) && !string.IsNullOrWhiteSpace(textBox6.Text) &&
                !string.IsNullOrEmpty(textBox5.Text) && !string.IsNullOrWhiteSpace(textBox5.Text))
            {
                SqlCommand command = new SqlCommand("INSERT INTO [Products] (Name, Prise, Weight, Date, Provider, Descriptoin)VALUES(@Name, @Prise, @Weight, @Date, @Provider, @Descriptoin)", sqlConnection);

                command.Parameters.AddWithValue("Name", textBox1.Text);
                command.Parameters.AddWithValue("Prise", textBox2.Text);
                command.Parameters.AddWithValue("Weight", textBox4.Text);
                command.Parameters.AddWithValue("Date", textBox3.Text);
                command.Parameters.AddWithValue("Provider", textBox6.Text);
                command.Parameters.AddWithValue("Descriptoin", textBox5.Text);

                await command.ExecuteNonQueryAsync();

            }
            else
            {
                label15.Visible = true;
                label15.Text = "Поля 'Название' , 'Цена' , 'Вес' , 'Дата' , 'Поставщик' , 'Описание' должны быть заполнены!";
            }
        }

        private async void обновитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();

            SqlDataReader sqlReader = null;

            SqlCommand command = new SqlCommand("SELECT * FROM [Products]", sqlConnection);

            try
            {
                sqlReader = await command.ExecuteReaderAsync();

                while (await sqlReader.ReadAsync())
                {
                    listBox1.Items.Add(Convert.ToString(sqlReader["Id"]) + "   " + Convert.ToString(sqlReader["Name"])
                        + "   " + Convert.ToString(sqlReader["Prise"]) + "   " + Convert.ToString(sqlReader["Weight"]) + "   " + Convert.ToString(sqlReader["Date"])
                        + "   " + Convert.ToString(sqlReader["Provider"]) + "   " + Convert.ToString(sqlReader["Descriptoin"]));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), ex.Source.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (sqlReader != null)
                    sqlReader.Close();
            }
        }

        private async void button2_Click(object sender, EventArgs e)
        {
            if (label16.Visible)
                label16.Visible = false;

            if (!string.IsNullOrEmpty(textBox7.Text) && !string.IsNullOrWhiteSpace(textBox7.Text) &&
                !string.IsNullOrEmpty(textBox8.Text) && !string.IsNullOrWhiteSpace(textBox8.Text) &&
                !string.IsNullOrEmpty(textBox9.Text) && !string.IsNullOrWhiteSpace(textBox9.Text) &&
                !string.IsNullOrEmpty(textBox10.Text) && !string.IsNullOrWhiteSpace(textBox10.Text) &&
                !string.IsNullOrEmpty(textBox11.Text) && !string.IsNullOrWhiteSpace(textBox11.Text) &&
                !string.IsNullOrEmpty(textBox12.Text) && !string.IsNullOrWhiteSpace(textBox12.Text) &&
                !string.IsNullOrEmpty(textBox13.Text) && !string.IsNullOrWhiteSpace(textBox13.Text))
            {
                SqlCommand command = new SqlCommand("UPDATE [Products] SET [Name]=@Name, [Prise]=@Prise, [Weight]=@Weight, [Date]=@Date, [Provider]=@Provider, [Descriptoin]=@Descriptoin WHERE [Id]=@Id", sqlConnection);

                command.Parameters.AddWithValue("Id", textBox13.Text);
                command.Parameters.AddWithValue("Name", textBox12.Text);
                command.Parameters.AddWithValue("Prise", textBox11.Text);
                command.Parameters.AddWithValue("Weight", textBox10.Text);
                command.Parameters.AddWithValue("Date", textBox9.Text);
                command.Parameters.AddWithValue("Provider", textBox8.Text);
                command.Parameters.AddWithValue("Descriptoin", textBox7.Text);

                await command.ExecuteNonQueryAsync();
            }
            else if (!string.IsNullOrEmpty(textBox13.Text) && !string.IsNullOrWhiteSpace(textBox13.Text))
            {
                label16.Visible = true;
                label16.Text = "Id должен быть заполнен!";
            }
            else
            {
                label16.Visible = true;
                label16.Text = "Поля 'Id' 'Название' , 'Цена' , 'Вес' , 'Дата' , 'Поставщик' , 'Описание' должны быть заполнены!";
            }

        }

        private async void button3_Click(object sender, EventArgs e)
        {
            if (label17.Visible)
                label17.Visible = false;

            if (!string.IsNullOrEmpty(textBox14.Text) && !string.IsNullOrWhiteSpace(textBox14.Text))
            {
                SqlCommand command = new SqlCommand("DELETE FROM [Products] WHERE [Id]=@Id", sqlConnection);

                command.Parameters.AddWithValue("Id", textBox14.Text);

                await command.ExecuteNonQueryAsync();
            }
            else
            {
                label17.Visible = true;
                label17.Text = "Id должен быть заполнен!";
            }
        }
    }
}
