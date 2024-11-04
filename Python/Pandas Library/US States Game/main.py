import turtle
import pandas
screen = turtle.Screen()
screen.title("US States Game")
image = "blank_states_img.gif"
screen.addshape(image)
turtle.shape(image)

data = pandas.read_csv("50_states.csv")
all_states = data.state.tolist()
guessed_states = []
user_has_left = False

while len(guessed_states) < 50 or user_has_left == False:
    answer_state = screen.textinput(f"{len(guessed_states)}/50 States Correct",
                                    "What is another state's name?").title()


    if answer_state in all_states:
        guessed_states.append(answer_state)
        t = turtle.Turtle()
        t.hideturtle()
        t.penup()
        t.color("Black")
        state_data = data[data.state == answer_state]
        t.goto(int(state_data.x.iloc[0]), int(state_data.y.iloc[0]))
        t.write(state_data.state.item())
    
    if answer_state == 'Exit':
        user_has_left =True
        states_to_guess = []
        states_to_guess_file_info = {}
        for state in all_states:
            if state not in guessed_states:                
                states_to_guess.append(state)
                
        states_to_guess = {
            
            "states_names" : states_to_guess
        }
        
        data_frame = pandas.DataFrame(states_to_guess)        
        data_frame.to_csv("states_to_guess.csv")
            
        
                

screen.exitonclick()



