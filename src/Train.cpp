#include "Train.h"

enum  optionIndex { UNKNOWN, HELP, PLUS };
const option::Descriptor usage[] =
{
 {UNKNOWN, 0, "", "",option::Arg::None, "USAGE: trainer <trainer_file> [options]\n\n"
                                        "Options:" },
 {PLUS, 0,"i","input-nodes",option::Arg::int, "  -i, --input-nodes \tNumber of input nodes." },
 {PLUS, 0,"h","hidden-nodes",option::Arg::int, "  -h, --hidden-nodes \tNumber of hidden nodes." },
 {PLUS, 0,"o","output-nodes",option::Arg::int, "  -o, --output-nodes \tNumber of output nodes." },
 {PLUS, 0,"l","hidden-layers",option::Arg::int, "  -h, --hidden-layers \tNumber of hidden layers." },
 {PLUS, 0,"w","weight-file",option::Arg::string, "  -w, --weight-file \tName of weight file." },
 {HELP, 0,"", "help",option::Arg::None, "  --help  \tPrint usage and exit." },
 {UNKNOWN, 0, "", "",option::Arg::None, "\nExamples:\n"
                               "  trainer trainer_file.tra -i 10 -h 5 -o 2 -h 1\n"
                               "  trainer \n" },
 {0,0,0,0,0,0}
};

// Returns the path to the default weights directory
std::string getWeightsDir(char* argvZero) {
  std::string executablePath = realpath(argvZero, NULL);
  size_t index = executablePath.rfind('/', executablePath.rfind('/') - 1);
  return executablePath.substr(0, index) + "/files/weights";
}

int main(int argc, char* argv[])
{

 argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  std::vector<option::Option> options(stats.options_max);
  std::vector<option::Option> buffer(stats.buffer_max);
  option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

  if (parse.error())
    return 1;

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }

  std::cout << "--plus count: " <<
      options[PLUS].count() << "\n";

  for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
    std::cout << "Unknown option: " << std::string(opt->name,opt->namelen) << "\n";

  for (int i = 0; i < parse.nonOptionsCount(); ++i)
    std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";


  Network myNeuralNet = Network(2, 5, 2, 1);
  //Network myNeuralNet = Network(10, 5, 7, 1);
  //Network myNeuralNet = Network(5, 5, 2, 1);

  static const float inputArr[] = {4.5, -2.3 };
  //static const float inputArr[] = {1.0,2.5,3.5,4.2,4.3,1.5,2.5,1.9,-1.0,-2.6};
  //static const float inputArr[] = {1.0,2.5,3.5,4.5,1.5};
  static const float outputArr[] = {0.99, 0.01};
  std::vector<float> inputVec (inputArr, inputArr + sizeof(inputArr) / sizeof(inputArr[0]) );
  std::vector<float> outputVec (outputArr, outputArr + sizeof(outputArr) / sizeof(outputArr[0]) );

  float totalError = 1.0;
  int iterations = 0;
  while (totalError > 0.0001) {
    iterations++;
    totalError = myNeuralNet.train(inputVec, outputVec);
  }
  std::cout << "===============\n";
  std::cout << "Iterations: " << iterations << std::endl;
  std::cout << "Total Error: " << totalError << std::endl;
  std::cout << "===============\n";

  myNeuralNet.writeWeightFile(getWeightsDir(argv[0]));
  return 0;
}
