import gym
env = gym.make('CarRacing-v0')
#Details about environment: https://github.com/openai/gym/blob/master/gym/envs/box2d/car_racing.py
env.reset()
total_reward = 1000
for i_episode in range(2):
    observation = env.reset()
    for t in range(1000000):
        env.render()

        #action = env.action_space.sample()
        #self.action_space = spaces.Box( np.array([-1,0,0]), np.array([+1,+1,+1]))  # steer, gas, brake
        #self.observation_space = spaces.Box(low=0, high=255, shape=(STATE_H, STATE_W, 3), dtype=np.uint8)
        #print(action)
        action=[0,1,0]  #forward, gas, no break
        observation, reward, done, info = env.step(action)
        print (reward, done, info)
        total_reward += reward

        #NOTE: Need to detect if off-the-road!!! car will continue to "drive" on grass
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            env.reset()
            total_reward += -1000
            break
env.close()