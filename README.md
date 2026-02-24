# GaTher3DEvo

Program2.c can be used to calculate the outcome of evolutionary game theory's payoff matrix, while GUI.py to visualize that outcome.

An XML file of the structure defined similarly to what is shown below is needed as an input to both programs:
```
<?xml version="1.0" encoding="UTF-8"?>
<data>
    <inputs> 
        <dimension>2</dimension><!--dimensions-->
        <size>10</size><!--size of input matrix-->
        <MSEG>0</MSEG><!--0 - spatial games; 1 - mixed games-->
        <maxgeneration>500</maxgeneration> <!--number of generations-->
        <mortality>0.1</mortality> <!--mortality, for semi-synchronous updating method only-->
        <updating>1</updating><!--0 - asynchronous;
                1 - semi-synchronous; 2 - synchronous-->
        <neighbourhooud>0</neighbourhood> <!--0 - von Neumann neighbourhood; 
                1 - Moore neighbourhood-->
        <reproduction>2</reproduction><!--0 - probabilistic;
                1 - deterministic; 2 - weighted mean-->
        <param>3</param><!--number of cells for mean, for weighted mean reproduction method only-->
        <resources v="6" c="9" param1="0.0" param2="1.0" param3="300">step</resources><!--resources function-->
        <payoff>
            <row r="0">2 1.5 4</row>
            <row r="1">3 1 0</row>
            <row r="2">3 1 3</row>
        </payoff><!--payoff matrix-->
        <init-population>binFile</init-population><!--bin matrix with initial population;
        if missing, initial population is randomized-->
    </inputs>
    <outputs>
        <save-path>outputFile</save-path><!--save path-->
    </outputs>
</data>
```

The following variables have to be specified:
- dimension - the number of dimensions;
- size - size of input matrix/game lattice;
- MSEG - whether to utilize spatial (0) or mixed games (1);
- maxgeneration - number of generations to run the algorithm for;
- mortality - chance for cells to die, for semi-synchronous updating method only;
- updating - updating method for cell mortality, asynchronous (0), semi-synchronous (1) and synchronous (2);
- neighbourhooud - whether to calculate outcome using von Neumann (0) or Moore (1) neighbourhooud;
- reproduction - reprooduction method for cells, probabilistic (0, the chosen cell is the one with highest fitness in the neighbourhooud); deterministic (1, the chosen cell is the one with highest fitness in the neighbourhooud divided by the total score of the neighbourhooud) and weighted mean (2, the cell to replace the dead one is calculated using weighted mean);
- param - number of cells for weighted mean reproduction only;
- resources v, c, param1, param2, param3,- for resources function, with v and c being variables used in Hawk-Dove game:
  - step - for step function, with param1 being initial value, param2 final value and param3 being time of change of value;
  - linear - for linear function, with param1 being variable a and param2 variable b in equation y = ax + b;
  - sine - for sine function, with param1 being amplitude, param2 mean value and param3 period;
  - globalH - global population of Hawks-dependant, param1 being the number of dependency function:
    1. $$r(H) = -H + 1$$;
    2. $$r(H) = -(H - 1) * (H + 1)$$;
    3. $$r(H) = 2/(H + 1) - 1$$;
    4. $$r(H) = cos(H * pi/2)$$.
  - localH - local population of Hawks-dependant, param1 being the number of dependency function:
    1. $$r(H) = -H + 1$$;
    2. $$r(H) = -(H - 1) * (H + 1)$$;
    3. $$r(H) = 2/(H + 1) - 1$$;
    4. $$r(H) = cos(H * pi/2)$$.
- payoff - payoff matrix;
- init-population - optional, bin file with initial population;
- save-path - save path.

Working example:
```
<?xml version="1.0" encoding="UTF-8"?>
<data>
	<inputs>
		<dimension>3</dimension>
		<size>10</size>
		<MSEG>0</MSEG>
		<maxgeneration>60</maxgeneration>
		<mortality>0.1</mortality>
		<updating>1</updating>
		<neighbourhood>1</neighbourhood>
		<reproduction>1</reproduction>
		<param>3</param>
		<payoff>
			<row r="0">0.75 3.50 1.50</row>
			<row r="1">8.00 0.50 4.00</row>
			<row r="2">6.50 5.50 0.00</row>
		</payoff>
	</inputs>
	<outputs>
		<save-path>RESULTS/</save-path>
	</outputs>
</data>

```

Initial population matrix is a bin matrix - for 3x3 game lattice for 2 phenotypes, its size is 3x3x2 (3x3 for one phenotype and 3x3 for the other, subsequently).

![GUI_input](https://github.com/user-attachments/assets/57c2bc3d-13bc-402a-9805-a44aca544dd3)

Input needed to start the program. Remember to put XML file in the same folder as program.

In order to use the graphic user interface, an input XML file must be provided, and the file path can be specified in two different ways, either by writing a Python program call and GUI file name into the terminal, after which the user will be asked to provide file path to XML file, or adding it after GUI file name.

![GUI_3D_MSEG](https://github.com/user-attachments/assets/a556101c-a642-4630-b186-af05e2c93fdc)

GUI for 3D MSEG model. Note the layers slider on the right.

GUI allows user to see average result of all generations, time course of phenotypes and 3D results for given generation.

![GUI_3D](https://github.com/user-attachments/assets/915cee55-190d-4fec-a565-02aa719cfad0)

3D matrix. You can rotate it, zoom in and out.

![show_time_course_of_phenotypes](https://github.com/user-attachments/assets/5950edd9-f260-4e76-8032-82962b782af2)

Show time course of phenotypes. It is not the same as replicator dynamics!

Software written by Marek Bonk as part of his Master's Thesis project on Silesian University of Technology.
