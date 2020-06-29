# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for 
# educational purposes provided that (1) you do not distribute or publish 
# solutions, (2) you retain this notice, and (3) you provide clear 
# attribution to UC Berkeley, including a link to 
# http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero 
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and 
# Pieter Abbeel (pabbeel@cs.berkeley.edu).
#
# Modified by Eugene Agichtein for CS325 Sp 2014 (eugene@mathcs.emory.edu)
#

from util import manhattanDistance
from game import Directions
import random
import util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        Note that the successor game state includes updates such as available food,
        e.g., would *not* include the food eaten at the successor state's pacman position
        as that food is no longer remaining.
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        currentFood = currentGameState.getFood() #food available from current state
        newFood = successorGameState.getFood() #food available from successor state (excludes food@successor) 
        currentCapsules=currentGameState.getCapsules() #power pellets/capsules available from current state
        newCapsules=successorGameState.getCapsules() #capsules available from successor (excludes capsules@successor)
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        score = successorGameState.getScore()

        foodDistances = [manhattanDistance(newPos, f) for f in newFood.asList()]
        if len(foodDistances):
            closestFood = min(foodDistances)
            score += 10.00 / closestFood

        ghostDistance = manhattanDistance(newPos, newGhostStates[0].getPosition())
        if ghostDistance > 0:
            score -= 10.00 / ghostDistance

        capDistances = [manhattanDistance(newPos, cap) for cap in newCapsules]
        if len(capDistances):
            closestCap = min(capDistances)
            score += 7.50 / closestCap

        closestGhost = 999999
        for gstate in newGhostStates:
            tmp = manhattanDistance(newPos, gstate.getPosition())
            if tmp < closestGhost and gstate.scaredTimer > 0:
                closestGhost = tmp

        score += 12.50 / closestGhost

        return score

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        def util(state, depth, agentIndex):
            if agentIndex == state.getNumAgents():
                agentIndex = 0
                depth += 1

            if depth > self.depth or state.isWin() or state.isLose():
                return self.evaluationFunction(state)
            elif agentIndex == 0:
                return maxvalue(state, depth, agentIndex)
            else:
                return minvalue(state, depth, agentIndex)

        def maxvalue(state, depth, agentIndex):
            v = -999999
            actions = [action for action in state.getLegalActions() if action != Directions.STOP]
            for action in actions:
                v = max(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1))

            return v

        def minvalue(state, depth, agentIndex):
            v = 999999
            for action in state.getLegalActions(agentIndex):
                v = min(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1))

            return v

        v = -999999
        actions = [action for action in gameState.getLegalActions() if action != Directions.STOP]
        bestAction = None
        for action in actions:
            tmp = util(gameState.generateSuccessor(0, action), 1, 1)
            if tmp > v:
                v = tmp
                bestAction = action

        return bestAction

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"

        def util(state, depth, agentIndex, a, b):
            if agentIndex == state.getNumAgents():
                agentIndex = 0
                depth += 1

            if depth > self.depth or state.isWin() or state.isLose():
                return self.evaluationFunction(state)
            elif agentIndex == 0:
                return maxvalue(state, depth, agentIndex, a, b)
            else:
                return minvalue(state, depth, agentIndex, a, b)

        def maxvalue(state, depth, agentIndex, a, b):
            v = -999999
            actions = [action for action in state.getLegalActions() if action != Directions.STOP]
            for action in actions:
                v = max(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1, a, b))
                if v > b:
                    return v

                a = max(a, v)

            return v

        def minvalue(state, depth, agentIndex, a, b):
            v = 999999
            for action in state.getLegalActions(agentIndex):
                v = min(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1, a, b))
                if v < a:
                    return v

                b = min(b, v)

            return v

        v = -999999
        a = -999999
        b = 999999
        actions = [action for action in gameState.getLegalActions() if action != Directions.STOP]
        bestAction = None
        for action in actions:
            tmp = util(gameState.generateSuccessor(0, action), 1, 1, a, b)
            a = max(a, tmp)
            if tmp > v:
                v = tmp
                bestAction = action

        return bestAction

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        def util(state, depth, agentIndex):
            if agentIndex == state.getNumAgents():
                agentIndex = 0
                depth += 1

            if depth > self.depth or state.isWin() or state.isLose():
                return self.evaluationFunction(state)
            elif agentIndex == 0:
                return maxvalue(state, depth, agentIndex)
            else:
                return evalue(state, depth, agentIndex)

        def maxvalue(state, depth, agentIndex):
            v = -999999
            actions = [action for action in state.getLegalActions() if action != Directions.STOP]
            for action in actions:
                v = max(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1))

            return v

        def evalue(state, depth, agentIndex):
            v = 0
            i = 0
            for action in state.getLegalActions(agentIndex):
                v += util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1)
                i += 1

            v /= i  # simply take the average of all values from all actions;
                    # same as having uniform probability for all actions
            return v

        v = -999999
        actions = [action for action in gameState.getLegalActions() if action != Directions.STOP]
        bestAction = None
        for action in actions:
            tmp = util(gameState.generateSuccessor(0, action), 1, 1)
            if tmp > v:
                v = tmp
                bestAction = action

        return bestAction

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: The evaluation function takes the closest food and the closest ghost and
      adds or subtracts the same weight times the inverse of each value, respectively. The inverse of the
      closest capsule distance is then multiplied by a weight of 75% of the previous weight.
      Finally, the inverse of the closest ghost whose scare timer is still > 0 is multiplied by a weight
      of 125% of the original weight. All values are added together for a final score.
    """
    "*** YOUR CODE HERE ***"
    newPos = currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()  # food available from successor state (excludes food@successor)
    newCapsules = currentGameState.getCapsules()  # capsules available from successor (excludes capsules@successor)
    newGhostStates = currentGameState.getGhostStates()
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

    score = currentGameState.getScore()

    foodDistances = [manhattanDistance(newPos, f) for f in newFood.asList()]
    if len(foodDistances):
        closestFood = min(foodDistances)
        score += 10.00 / closestFood

    closestGhost = 999999
    for gstate in newGhostStates:
        tmp = manhattanDistance(newPos, gstate.getPosition())
        if tmp < closestGhost:
            closestGhost = tmp

    ghostDistance = manhattanDistance(newPos, newGhostStates[0].getPosition())
    if ghostDistance > 0:
        score -= 10.00 / ghostDistance

    closestCap = 999999
    capDistances = [manhattanDistance(newPos, cap) for cap in newCapsules]
    length = len(capDistances)
    if length:
        closestCap = min(capDistances)

    closestScaredGhost = 999999
    cnt = 0
    for gstate in newGhostStates:
        tmp = manhattanDistance(newPos, gstate.getPosition())
        if tmp < closestScaredGhost and gstate.scaredTimer > 0:
            closestScaredGhost = tmp
            cnt += 1

    if (cnt > 0 and length > 0) or closestScaredGhost < closestGhost:
        score += 12.50 / closestScaredGhost
        score += 7.50 / closestCap
    elif closestCap < closestScaredGhost:
        score -= 7.50 / closestScaredGhost
        score -= 12.50 / closestCap

    return score

# Abbreviation
better = betterEvaluationFunction

class ContestAgent(MultiAgentSearchAgent):
    """
      Your agent for the mini-contest
    """

    def getAction(self, gameState):
        """
          Returns an action.  You can use any method you want and search to any depth you want.
          Just remember that the mini-contest is timed, so you have to trade off speed and computation.

          Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
          just make a beeline straight towards Pacman (or away from him if they're scared!)
        """
        "*** YOUR CODE HERE ***"
        def util(state, depth, agentIndex, a, b):
            if agentIndex == state.getNumAgents():
                agentIndex = 0
                depth += 1

            if depth > 2 or state.isWin() or state.isLose():
                return betterEvaluationFunction(state)
            elif agentIndex == 0:
                return maxvalue(state, depth, agentIndex, a, b)
            else:
                return minvalue(state, depth, agentIndex, a, b)

        def maxvalue(state, depth, agentIndex, a, b):
            v = -999999
            actions = [action for action in state.getLegalActions() if action != Directions.STOP]
            for action in actions:
                v = max(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1, a, b))
                if v > b:
                    return v

                a = max(a, v)

            return v

        def minvalue(state, depth, agentIndex, a, b):
            v = 999999
            for action in state.getLegalActions(agentIndex):
                v = min(v, util(state.generateSuccessor(agentIndex, action), depth, agentIndex + 1, a, b))
                if v < a:
                    return v

                b = min(b, v)

            return v

        v = -999999
        a = -999999
        b = 999999
        actions = [action for action in gameState.getLegalActions() if action != Directions.STOP]
        bestAction = None
        for action in actions:
            tmp = util(gameState.generateSuccessor(0, action), 1, 1, a, b)
            a = max(a, tmp)
            if tmp > v:
                v = tmp
                bestAction = action

        return bestAction


