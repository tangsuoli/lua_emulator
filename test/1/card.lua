card_id = 1
ans = {6, 7, 8, 9, 9, 8}
q_auido_files = {"1","2","3","4","5","6"}
q_cnt = 6
q_index = 1

function verify_answer(value, index)
	if (value == ans[index])
	then
		result = 1
	else
		result = 0
	end
	if (result == 1)
	then
		print("answer correct")
		c_play_sound(card_id, "cor")
	else
		c_play_sound(card_id, "wro")
		print("answer wrong, continue")
	end
	return result
end

break = 0
function main()
	c_play_sound(card_id, "guide")
	print("questions count:", q_cnt)
	while (break == 0)
	do
		if (q_index > q_cnt)
		then
			print("all finished")
			c_play_sound(card_id, "fin")
			break = 1
		else
            			print("play question")
            			c_play_sound(card_id, q_auido_files[q_index])
            			g_ret = c_get_input(5)
           			if (g_ret == -1) -- timeout
            			then
                			print("input timeout")
                			c_play_sound(card_id, "tout")
           		 	else
                			v_ret = verify_answer(g_ret, q_index)
                			if (v_ret == 1)
                			then
                    				q_index = q_index+1
               				end
           			end
        		end
	end
end

main()
 


