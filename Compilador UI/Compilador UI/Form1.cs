using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Compilador_UI
{
    public partial class MyCompiler : Form
    {
        CoSy.cManager Compilador;

        public MyCompiler()
        {
            InitializeComponent();
            Compilador = new CoSy.cManager();
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }
        private void quickSave()
        {

        }
        private void saveCode()
        {
            SaveFileDialog SaverDialog =new SaveFileDialog();
            SaverDialog.Filter = "TextFile.txt| *.txt";
            SaverDialog.Title = "Save File";
        

            if (SaverDialog.ShowDialog() == DialogResult.OK)
            {
                StreamWriter write = new StreamWriter(File.Create(SaverDialog.FileName));
                write.Write(codeBox.Text);
                write.Dispose();
            }

        }

        private void newFileTool(object sender, EventArgs e)
        { 
            if(codeBox.Text.Trim() != "")
            {
                DialogResult answer = MessageBox.Show("Guardar?", "Guardar?", MessageBoxButtons.YesNoCancel);
                if(answer == DialogResult.Yes)
                {
                    saveCode();
                    codeBox.Clear();
                }
                else if (answer == DialogResult.No)
                {
                    codeBox.Clear();
                }


            }
            
        }

        private void compileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            string[] details = Compilador.Compile(codeBox.Text);
            output.Lines = details;
            string[] tokens = Compilador.GetAllTokens();
            
            output.Lines = tokens;
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveCode();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Filter = "Text file(.txt)|*.txt| All Files(*.*)|*.*";
            openFile.FilterIndex = 1;
            openFile.Title = "Open File";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                StreamReader reader = new StreamReader(File.OpenRead(openFile.FileName));
                codeBox.Text = reader.ReadToEnd();
                reader.Dispose();
            }
        }
    }
}
