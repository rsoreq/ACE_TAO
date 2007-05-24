// $Id$

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(BiDirectional, server, "$Id$")

const char *ior_output_file = 0;
int no_iterations = 10;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        no_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-i <no_iterations>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i *server_impl = 0;

      ACE_NEW_THROW_EX (server_impl,
                        Simple_Server_i (orb.in (),
                                         no_iterations),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var owner_transfer (server_impl);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("simple_server");

      child_poa->activate_object_with_id (id.in (),
                                          server_impl);

      CORBA::Object_var obj =
        child_poa->id_to_reference (id.in ());

      CORBA::String_var ior =
        orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Run the event loop
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1, 1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
