import sys
import os
# print(os.path.dirname(os.path.realpath(__file__)))
from llama_cpp import Llama

model_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "llm-model.gguf")
def main(input_data):
    llm = Llama(
        model_path=model_path,
        n_ctx=4048,
        n_threads=4,
        n_gpu_layers=0,
        chat_format="chatml"
    )

    system_message = "You are an AI tasked with analyzing form results to provide a personal morality assessment based on the entirety of a user's responses. Your analysis should synthesize the results into a comprehensive psychological profile, highlighting key traits and tendencies in a clear, concise manner. Present your findings in a summarized format, akin to a professional psychological assessment. The response should be no longer than three brief paragraphs and must not exceed 200 words. Use standard text with paragraphs separated by newlines only."
    prompt_template = f'''system
{system_message}
user
{input_data}
assistant
'''

    output = llm(
        prompt_template,
        max_tokens=512,
        stop=["</s>"],
        echo=True
    )

    print("The whole result: %%"+str(output['choices'])+"%%")

    result = str(output['choices'][0]['text']).split('assistant\n')[-1]
    print("##split#here##"+result)

if __name__ == "__main__":
    input_data = sys.stdin.read().strip()
    main(input_data)
